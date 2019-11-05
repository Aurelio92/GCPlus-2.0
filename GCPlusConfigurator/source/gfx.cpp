#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <gccore.h>
#include <list>
#include "gfx.h"

#define DEFAULT_FIFO_SIZE   (256 * 1024)

namespace Gfx
{
    static void* frameBuffer[2] = {NULL, NULL};
    GXRModeObj* rmode;

    f32 yscale;

    u32 xfbHeight;

    Mtx viewMatrix;
    Mtx44 projMatrix;
    Mtx model, modelview;

    typedef struct {
        Mtx mtx;
    } MtxContainer;

    u32    fb = 0;     // initial framebuffer index

    std::list<MtxContainer> viewMatrixStack;
    std::list<Rect> scissorBoxStack;


    void init() {
        VIDEO_Init();
        rmode = &TVNtsc480Prog; //VIDEO_GetPreferredMode(NULL);

        // 16:9 and 4:3 Screen Adjustment
        if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
            rmode->viWidth = 640;
            rmode->viXOrigin = (VI_MAX_WIDTH_NTSC - 640)/2;  // This probably needs to consider PAL
        } else {    // 4:3
            rmode->viWidth = 640;
            rmode->viXOrigin = (VI_MAX_WIDTH_NTSC - 640)/2;
        }

        // allocate 2 framebuffers for double buffering
        frameBuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
        frameBuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

        VIDEO_Configure(rmode);
        VIDEO_SetNextFramebuffer(frameBuffer[fb]);
        VIDEO_SetBlack(FALSE);
        VIDEO_Flush();
        VIDEO_WaitVSync();
        if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

        // setup the fifo and then init the flipper
        void *gp_fifo = NULL;
        gp_fifo = memalign(32, DEFAULT_FIFO_SIZE);
        memset(gp_fifo, 0, DEFAULT_FIFO_SIZE);

        GX_Init(gp_fifo, DEFAULT_FIFO_SIZE);

        // clears the bg to color and clears the z buffer
        GX_SetCopyClear((GXColor){0x28, 0x29, 0x23, 0xff}, 0x00ffffff);

        // other gx setup
        GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
        yscale = GX_GetYScaleFactor(rmode->efbHeight, rmode->xfbHeight);
        xfbHeight = GX_SetDispCopyYScale(yscale);
        GX_SetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
        GX_SetDispCopyDst(rmode->fbWidth, xfbHeight);
        GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
        GX_SetFieldMode(rmode->field_rendering, ((rmode->viHeight == 2 * rmode->xfbHeight) ? GX_ENABLE : GX_DISABLE));

        if (rmode->aa) {
            GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
        } else {
            GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
        }

        GX_SetCullMode(GX_CULL_NONE);
        GX_CopyDisp(frameBuffer[fb], GX_TRUE);
        GX_SetDispCopyGamma(GX_GM_1_0);

        GX_ClearVtxDesc();
        GX_InvVtxCache ();
        GX_InvalidateTexAll();

        GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
        GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
        GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);


        GX_SetVtxAttrFmt (GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GX_SetVtxAttrFmt (GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GX_SetZMode (GX_FALSE, GX_LEQUAL, GX_TRUE);

        GX_SetNumChans(1);
        GX_SetNumTexGens(1);
        GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
        GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

        guMtxIdentity(viewMatrix);
        guMtxTransApply(viewMatrix, viewMatrix, 0.0f, 0.0f, -100.0f);
        GX_LoadPosMtxImm(viewMatrix, GX_PNMTX0);

        if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
            guOrtho(projMatrix, 0.0f, rmode->efbHeight, 0.0f, 800, 0.0f, 1000.0f);
        } else {
            guOrtho(projMatrix, 0.0f, rmode->efbHeight, 0.0f, 640, 0.0f, 1000.0f);
        }
        GX_LoadProjectionMtx(projMatrix, GX_ORTHOGRAPHIC);

        GX_SetViewport(0.0f, 0.0f, rmode->fbWidth, rmode->efbHeight, 0.0f, 1.0f);
        GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GX_SetAlphaUpdate(GX_TRUE);
        GX_SetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GX_SetColorUpdate(GX_ENABLE);
        GX_SetCullMode(GX_CULL_NONE);

        //Initialize scissor box
        GX_SetClipMode(GX_CLIP_ENABLE);
        GX_SetScissor(0, 0, rmode->fbWidth, rmode->efbHeight);
        Rect box(0, 0, rmode->fbWidth, rmode->efbHeight);
        scissorBoxStack.push_back(box);
    }

    void startDrawing() {
        GX_SetViewport(0.0f, 0.0f, rmode->fbWidth, rmode->efbHeight, 0.0f, 1.0f);

        viewMatrixStack.clear();
        guMtxIdentity(model);
        guMtxConcat(viewMatrix, model, modelview);
        MtxContainer mc;
        memcpy(mc.mtx, modelview, sizeof(Mtx));
        viewMatrixStack.push_back(mc);
        // load the modelview matrix into matrix memory
        GX_LoadPosMtxImm(modelview, GX_PNMTX0);
    }

    void fillScreen() {
        VIDEO_ClearFrameBuffer(rmode, frameBuffer[fb], COLOR_GREEN);
    }

    void endDrawing() {
        GX_LoadPosMtxImm(viewMatrix, GX_PNMTX0);

        //Do this stuff after drawing
        GX_DrawDone();

        fb ^= 1; //flip framebuffer
        GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        GX_SetColorUpdate(GX_TRUE);
        GX_CopyDisp(frameBuffer[fb],GX_TRUE);

        VIDEO_SetNextFramebuffer(frameBuffer[fb]);

        VIDEO_Flush();

        VIDEO_WaitVSync();
        // Interlaced screens require two frames to update
        if (rmode->viTVMode &VI_NON_INTERLACE)  VIDEO_WaitVSync();
    }

    void concatViewMatrix(Mtx mi, Mtx mo) {
        guMtxConcat(viewMatrix, mi, mo);
    }

    void restoreViewMatrix() {
        guMtxIdentity(viewMatrix);
        guMtxTransApply(viewMatrix, viewMatrix, 0.0f, 0.0f, -100.0f);
        GX_LoadPosMtxImm(viewMatrix, GX_PNMTX0);
    }

    void translate(f32 x, f32 y) {
        //guMtxIdentity(viewMatrix);
        MtxContainer mc = viewMatrixStack.back();
        guMtxTransApply(mc.mtx, mc.mtx, x, y, 0.0f);
        viewMatrixStack.pop_back();
        viewMatrixStack.push_back(mc);
        GX_LoadPosMtxImm(mc.mtx, GX_PNMTX0);
    }

    void pushMatrix() {
        viewMatrixStack.push_back(viewMatrixStack.back());
    }

    void popMatrix() {
        viewMatrixStack.pop_back();
        MtxContainer mc = viewMatrixStack.back();
        GX_LoadPosMtxImm(mc.mtx, GX_PNMTX0);
    }

    void getCurMatrix(Mtx out) {
        MtxContainer mc = viewMatrixStack.back();
        memcpy(out, mc.mtx, sizeof(Mtx));
    }

    Rect getCurScissorBox() {
        return scissorBoxStack.back();
    }

    void pushScissorBox(u32 w, u32 h) {
        MtxContainer mc = viewMatrixStack.back();
        Rect oldBox = scissorBoxStack.back();
        if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
            w = w * 640 / 800;
            mc.mtx[0][3] = mc.mtx[0][3] * 640 / 800;
        }
        Rect newBox((int)mc.mtx[0][3], (int)mc.mtx[1][3], w, h);
        Rect outBox;
        oldBox.intersection(outBox, newBox);
        scissorBoxStack.push_back(oldBox);
        GX_SetScissor(outBox.x, outBox.y, outBox.width, outBox.height);
    }

    void popScissorBox() {
        scissorBoxStack.pop_back();
        Rect box = scissorBoxStack.back();
        GX_SetScissor(box.x, box.y, box.width, box.height);
    }
}
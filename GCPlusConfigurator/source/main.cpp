#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <ogc/lwp_watchdog.h>
#include <ogc/machine/processor.h>
#include <wiiuse/wpad.h>
#include <ogc/ipc.h>
#include <fat.h>
#include <math.h>
#include <time.h>

#include "gfx.h"
#include "draw.h"
#include "font.h"
#include "animation.h"
#include "vector2.h"

#include "Menlo-Regular_ttf.h"
#include "SFMono-Regular_otf.h"
#include "FSEX300_ttf.h"

#include "textures.h"
#include "textures_tpl.h"

#define PI 3.14159265359f

void drawTopBar(Font& font);
void drawBottomBar(Font& font);
u8 mainMenuLoop(Font& font);

int main(int argc, char **argv) {
    TPLFile tdf;
    Gfx::init();
    PAD_Init();
    WPAD_Init();

    fatInitDefault();

    Font FSEX300(FSEX300_ttf, FSEX300_ttf_size, 20);
    Font menlo(Menlo_Regular_ttf, Menlo_Regular_ttf_size, 20);
    Font sfmono(SFMono_Regular_otf, SFMono_Regular_otf_size, 20);

    TPL_OpenTPLFromMemory(&tdf, (void*)textures_tpl, textures_tpl_size);
    Texture tex1 = createTextureFromTPL(&tdf, 0);
    Texture tex2 = createTextureFromTPL(&tdf, 1);

    Animation<Vector2> anim;
    Vector2 xy;
    Vector2 xy1;
    Vector2 xy2;
    anim.setOutput(&xy);
    xy1 = Vector2(0, 0);
    xy2 = Vector2(0, 0);
    anim.addStep(millisecs_to_ticks(1000), xy1, xy2);
    xy1 = Vector2(0, 0);
    xy2 = Vector2(48, 0);
    anim.addStep(millisecs_to_ticks(100), xy1, xy2);
    xy1 = Vector2(48, 0);
    xy2 = Vector2(48, 0);
    anim.addStep(millisecs_to_ticks(1000), xy1, xy2);

    for (int i = 0; i < 99; i++) {
        float x1 = 48 * cos(i * 2 * PI / 100);
        float y1 = 48 * sin(i * 2 * PI / 100);
        float x2 = 48 * cos((i + 1) * 2 * PI / 100);
        float y2 = 48 * sin((i + 1) * 2 * PI / 100);
        xy1 = Vector2(x1, y1);
        xy2 = Vector2(x2, y2);
        anim.addStep(millisecs_to_ticks(10), xy1, xy2);
    }
    xy1 = Vector2(48, 0);
    xy2 = Vector2(48, 0);
    anim.addStep(millisecs_to_ticks(300), xy1, xy2);
    anim.addReturnToHomeStep(millisecs_to_ticks(100));
    anim.resume();

    srand(time(NULL));

    u8 selected = 0;

    while (1) {
        PAD_ScanPads();
        WPAD_ScanPads();

        int down = PAD_ButtonsDown(0);
        if (down & PAD_BUTTON_START) exit(0);

        anim.animate();

        Gfx::startDrawing();
        Gfx::pushMatrix();
        Gfx::translate(100, 100);
        drawTextureResized(0, 0, 128, 128, tex2);
        drawTextureResized(xy[0], xy[1], 128, 128, tex1);
        Gfx::popMatrix();
        Gfx::endDrawing();

        /*if (selected == 0) {
            selected = mainMenuLoop(sfmono);
        }*//* else if (selected == 1) {
            int ret = powerMiiSettingsMenuLoop(sfmono, powerMiiConfig);
            if (ret == 1) { //Save config
                powerMiiWriteConfig(&powerMiiConfig, &powerMiiConfigBackup);
                powerMiiConfigBackup = powerMiiConfig;
                selected = 0;
            } else if (ret == -1) { //Discard config
                powerMiiConfig = powerMiiConfigBackup;
                selected = 0;
            }
        } else if (selected == 2) {
            int ret = chargeSettingsMenuLoop(sfmono, powerMiiConfig);
            if (ret == 1) { //Save config
                powerMiiWriteConfig(&powerMiiConfig, &powerMiiConfigBackup);
                powerMiiConfigBackup = powerMiiConfig;
                selected = 0;
            } else if (ret == -1) { //Discard config
                powerMiiConfig = powerMiiConfigBackup;
                selected = 0;
            }
        } else if (selected == 3) {
            if (fanSettingsMenuLoop(sfmono, timeAx, temperature, fanSpeed, tempPlot, fanSettings)) {
                selected = 0;
            }
        } else if (selected == 4) {
            if (updateMenuLoop(sfmono, updateHex.binary, updateHex.binarySize)) {
                selected = 0;
            }
        } else {
            selected = 0;
        }*/

    }

    return 0;

}

void drawTopBar(Font& font) {
    if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
        Gfx::pushMatrix();
        drawRectangle(0, 0, 800, 68, RGBA8(0x40, 0x40, 0x40, 0xFF));
        Gfx::translate(0, 16);
        font.printf(12, 12, L"GC+2.0 configurator");
        Gfx::popMatrix();
    } else {
        Gfx::pushMatrix();
        drawRectangle(0, 0, 640, 68, RGBA8(0x40, 0x40, 0x40, 0xFF));
        Gfx::translate(0, 16);
        font.printf(12, 12, L"GC+2.0 configurator");
        Gfx::popMatrix();
    }
}

void drawBottomBar(Font& font) {
    u16 ver = (1 << 13);
    u16 maj = (ver >> 13) & 0x1FFF;
    u16 min = ver & 0x0007;
    if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
        Gfx::pushMatrix();
        drawRectangle(0, 436, 800, 44, RGBA8(0x40, 0x40, 0x40, 0xFF));
        Gfx::translate(0, 440);
        font.printf(12, 0, L"v%u.%u", maj, min);
        Gfx::popMatrix();
    } else {
        Gfx::pushMatrix();
        drawRectangle(0, 436, 640, 44, RGBA8(0x40, 0x40, 0x40, 0xFF));
        Gfx::translate(0, 440);
        font.printf(12, 0, L"v%u.%u", maj, min);
        Gfx::popMatrix();
    }
}

u8 mainMenuLoop(Font& font) {
    static int selected = 0;
    int down = PAD_ButtonsDown(0);

    if (down & PAD_BUTTON_UP) {
        if (selected >= 3) {
            selected -= 3;
        }
    }

    if (down & PAD_BUTTON_DOWN) {
        if (selected < 3) {
            selected += 3;
        }
    }

    if (down & PAD_BUTTON_LEFT) {
        if ((selected % 3) > 0) {
            selected--;
        }
    }

    if (down & PAD_BUTTON_RIGHT) {
        if ((selected % 3) < 2) {
            selected++;
        }
    }

    if (down & PAD_BUTTON_A) {
        return selected + 1;
    }

    Gfx::startDrawing();
        //Drawing code goes here
        drawTopBar(font);
        drawBottomBar(font);
        if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
            Gfx::pushMatrix();
            Gfx::translate(0, 68);

            Gfx::pushMatrix();
            Gfx::translate((selected % 3) * 249, (selected / 3) * 172);
            drawRectangle(48, 20, 205, 156, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));
            Gfx::popMatrix();

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 2; j++) {
                    Gfx::pushMatrix();
                    Gfx::translate(i * 249, j * 172);
                    drawRectangle(52, 24, 197, 148, RGBA8(0x80, 0x80, 0x80, 0xFF));
                    Gfx::popMatrix();
                }
            }

            Gfx::pushMatrix();
            Gfx::translate(0, 0);
            Gfx::pushMatrix();
            Gfx::translate(52 + (197 - font.getTextWidth("Sticks wizard")) / 2, 24 + (148 - font.getSize()) / 2);
            font.printf(0, 0, "Sticks wizard");
            Gfx::popMatrix();
            Gfx::popMatrix();

            Gfx::pushMatrix();
            Gfx::translate(249, 0);
            Gfx::pushMatrix();
            Gfx::translate(52 + (197 - font.getTextWidth("Buttons testing")) / 2, 24 + (148 - font.getSize()) / 2);
            font.printf(0, 0, "Buttons testing");
            Gfx::popMatrix();
            Gfx::popMatrix();

            Gfx::pushMatrix();
            Gfx::translate(2 * 249, 0);
            Gfx::pushMatrix();
            Gfx::translate(52 + (197 - font.getTextWidth("Temp. control")) / 2, 24 + (148 - font.getSize()) / 2);
            font.printf(0, 0, "Temp. control");
            Gfx::popMatrix();
            Gfx::popMatrix();

            Gfx::pushMatrix();
            Gfx::translate(0, 172);
            Gfx::pushMatrix();
            Gfx::translate(52 + (197 - font.getTextWidth("Update")) / 2, 24 + (148 - font.getSize()) / 2);
            font.printf(0, 0, "Update");
            Gfx::popMatrix();
            Gfx::popMatrix();

            Gfx::popMatrix();
        } else {
            Gfx::pushMatrix();
            Gfx::translate(0, 68);

            Gfx::pushMatrix();
            Gfx::translate((selected % 3) * 209, (selected / 3) * 172);
            drawRectangle(8, 20, 205, 156, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));
            Gfx::popMatrix();

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 2; j++) {
                    Gfx::pushMatrix();
                    Gfx::translate(i * 209, j * 172);
                    drawRectangle(12, 24, 197, 148, RGBA8(0x80, 0x80, 0x80, 0xFF));
                    Gfx::popMatrix();
                }
            }
            Gfx::pushMatrix();
            Gfx::translate(0, 0);
            Gfx::pushMatrix();
            Gfx::translate(12 + (197 - font.getTextWidth("Sticks wizard")) / 2, 24 + (148 - font.getSize()) / 2);
            font.printf(0, 0, "Sticks wizard");
            Gfx::popMatrix();
            Gfx::popMatrix();

            Gfx::pushMatrix();
            Gfx::translate(209, 0);
            Gfx::pushMatrix();
            Gfx::translate(12 + (197 - font.getTextWidth("Buttons testing")) / 2, 24 + (148 - font.getSize()) / 2);
            font.printf(0, 0, "Buttons testing");
            Gfx::popMatrix();
            Gfx::popMatrix();

            Gfx::pushMatrix();
            Gfx::translate(2 * 209, 0);
            Gfx::pushMatrix();
            Gfx::translate(12 + (197 - font.getTextWidth("Temp. control")) / 2, 24 + (148 - font.getSize()) / 2);
            font.printf(0, 0, "Temp. control");
            Gfx::popMatrix();
            Gfx::popMatrix();

            Gfx::pushMatrix();
            Gfx::translate(0, 172);
            Gfx::pushMatrix();
            Gfx::translate(12 + (197 - font.getTextWidth("Update")) / 2, 24 + (148 - font.getSize()) / 2);
            font.printf(0, 0, "Update");
            Gfx::popMatrix();
            Gfx::popMatrix();

            Gfx::popMatrix();
        }
    Gfx::endDrawing();

    return 0;
}

#pragma once

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <libbbgui.h>
#include "textures.h"
#include "textures_pressed.h"
#include "gcplus.h"
#include "log.h"

class TestMenu : public GuiElement {
    private:
        Font* font;
        Font* fontSmall;
        u32 selected;
        int textCenterOffsetY;

        GuiImage* outline;
        GuiImage* A_outline;
        GuiImage* B_outline;
        GuiImage* CStick_outline;
        GuiImage* Start_outline;
        GuiImage* Stick_outline;
        GuiImage* X_outline;
        GuiImage* Y_outline;
        GuiImage* Dpad_outline;
        GuiImage* L_outline;
        GuiImage* R_outline;
        GuiImage* Z_outline;
        GuiImage* Dpad_full;
        GuiImage* A_pressed;
        GuiImage* B_pressed;
        GuiImage* CStick_pressed;
        GuiImage* Start_pressed;
        GuiImage* Stick_pressed;
        GuiImage* X_pressed;
        GuiImage* Y_pressed;
        GuiImage* Dpad_pressed;
        GuiImage* L_pressed;
        GuiImage* L_analog;
        GuiImage* R_pressed;
        GuiImage* R_analog;
        GuiImage* Z_pressed;

        void (*exitCallback)(GuiElement*);

        int buttonsHeld;
        float sx, sy, cx, cy;
        float ra, la;

    public:
        TestMenu(TPLFile* tdf, TPLFile* tdf_pressed, Font* _font, Font* _fontSmall, int _width, int _height) {
            font = _font;
            width = _width;
            height = _height;
            fontSmall = _fontSmall;

            if (font)
                textCenterOffsetY = font->getSize() - font->getCharHeight('O') / 2;

            //Load images
            outline = new GuiImage(tdf, TEXID_Outline);
            Start_outline = new GuiImage(tdf, TEXID_Start_outline);
            A_outline = new GuiImage(tdf, TEXID_A_outline);
            B_outline = new GuiImage(tdf, TEXID_B_outline);
            CStick_outline = new GuiImage(tdf, TEXID_CStick_outline);
            Stick_outline = new GuiImage(tdf, TEXID_Stick_outline);
            X_outline = new GuiImage(tdf, TEXID_X_outline);
            Y_outline = new GuiImage(tdf, TEXID_Y_outline);
            Dpad_outline = new GuiImage(tdf, TEXID_dpad_outline);
            L_outline = new GuiImage(tdf, TEXID_L_outline);
            R_outline = new GuiImage(tdf, TEXID_R_outline);
            Z_outline = new GuiImage(tdf, TEXID_Z_outline);

            A_pressed = new GuiImage(tdf_pressed, TEXID_A_pressed);
            B_pressed = new GuiImage(tdf_pressed, TEXID_B_pressed);
            CStick_pressed = new GuiImage(tdf_pressed, TEXID_CStick_pressed);
            Start_pressed = new GuiImage(tdf_pressed, TEXID_Start_pressed);
            Stick_pressed = new GuiImage(tdf_pressed, TEXID_Stick_pressed);
            X_pressed = new GuiImage(tdf_pressed, TEXID_X_pressed);
            Y_pressed = new GuiImage(tdf_pressed, TEXID_Y_pressed);
            Dpad_pressed = new GuiImage(tdf_pressed, TEXID_dpad_pressed);
            Dpad_full = new GuiImage(tdf_pressed, TEXID_dpad_full);
            L_pressed = new GuiImage(tdf_pressed, TEXID_L_pressed);
            L_analog = new GuiImage(tdf_pressed, TEXID_L_analog);
            R_pressed = new GuiImage(tdf_pressed, TEXID_R_pressed);
            R_analog = new GuiImage(tdf_pressed, TEXID_R_analog);
            Z_pressed = new GuiImage(tdf_pressed, TEXID_Z_pressed);
        }

        ~TestMenu() {
            delete outline;
            delete A_outline;
            delete B_outline;
            delete CStick_outline;
            delete Start_outline;
            delete Stick_outline;
            delete X_outline;
            delete Y_outline;
            delete Dpad_outline;
            delete Dpad_full;
            delete A_pressed;
            delete B_pressed;
            delete CStick_pressed;
            delete Start_pressed;
            delete Stick_pressed;
            delete X_pressed;
            delete Y_pressed;
            delete Dpad_pressed;
        }

        void draw(bool onFocus);
        void handleInputs();
        Vector2 getDimensions() {return Vector2(width, height);}

        void setExitCallback(void (*cb)(GuiElement*)) {
            exitCallback = cb;
        }
};

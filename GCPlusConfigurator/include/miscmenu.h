#pragma once

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <libbbgui.h>
#include "textures.h"
#include "textures_tpl.h"
#include "gcplus.h"
#include "log.h"

class MiscMenu : public GuiElement {
    private:
        Font* font;
        u32 selected;
        int textCenterOffsetY;

        GuiImage* leftStickBaseImg;
        GuiImage* leftStickImg;
        GuiImage* CStickBaseImg;
        GuiImage* CStickImg;

        void (*exitCallback)(GuiElement*);

        u8 miscConfig[5];

    public:
        MiscMenu(TPLFile* tdf, Font* _font, int _width, int _height) {
            font = _font;
            width = _width;
            height = _height;

            if (font)
                textCenterOffsetY = font->getSize() - font->getCharHeight('O') / 2;

            //Load sticks images
            leftStickBaseImg = new GuiImage(tdf, TEXID_LeftStick_base);
            leftStickBaseImg->setSize(128, 128);
            leftStickImg = new GuiImage(tdf, TEXID_LeftStick);
            leftStickImg->setSize(128, 128);
            CStickBaseImg = new GuiImage(tdf, TEXID_CStick_base);
            CStickBaseImg->setSize(128, 128);
            CStickImg = new GuiImage(tdf, TEXID_CStick);
            CStickImg->setSize(128, 128);

            //Read configuration from GC+2.0
            GCPlus::unlock();
            GCPlus::readEEPROM(0x13, miscConfig, 5);
            GCPlus::lock();

            if (miscConfig[2] > 1)
                miscConfig[2] = 1;
            if (miscConfig[4] > 1)
                miscConfig[4] = 0;

            #ifdef DEMOBUILD
            miscConfig[0] = 16;
            miscConfig[1] = 16;
            miscConfig[2] = 1;
            miscConfig[3] = 255;
            miscConfig[4] = 0;
            #endif
        }

        ~MiscMenu() {
            delete leftStickBaseImg;
            delete leftStickImg;
            delete CStickBaseImg;
            delete CStickImg;
        }

        void draw(bool onFocus);
        void handleInputs();
        Vector2 getDimensions() {return Vector2(width, height);}

        void setExitCallback(void (*cb)(GuiElement*)) {
            exitCallback = cb;
        }

        void onActiveEvent() {
            //Read configuration from GC+2.0
            GCPlus::unlock();
            GCPlus::readEEPROM(0x13, miscConfig, 5);
            GCPlus::lock();

            if (miscConfig[2] > 1)
                miscConfig[2] = 1;
            if (miscConfig[4] > 1)
                miscConfig[4] = 0;
        }
};

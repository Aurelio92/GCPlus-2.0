#pragma once

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <libbbgui.h>
#include "textures.h"
#include "textures_tpl.h"
#include "gcplus.h"
#include "log.h"

class WizardMenu : public GuiElement {
    private:
        enum fsmStates {
            LS_WAIT_FOR_MOVE = 0,
            LS_WAIT_FOR_ROT1, //60 degrees
            LS_WAIT_FOR_ROT2, //180
            LS_WAIT_FOR_ROT3, //240
            LS_WAIT_FOR_ROT4, //360
            LS_WAIT_FOR_RELEASE,
            CS_WAIT_FOR_MOVE,
            CS_WAIT_FOR_ROT1, //60 degrees
            CS_WAIT_FOR_ROT2, //180
            CS_WAIT_FOR_ROT3, //240
            CS_WAIT_FOR_ROT4, //360
            CS_WAIT_FOR_RELEASE,
            STICKS_TEST,
        };

        Font* font;
        fsmStates state;
        GuiImage* leftStickBaseImg;
        GuiImage* leftStickImg;
        GuiImage* CStickBaseImg;
        GuiImage* CStickImg;

        Animation<Vector2> stickAnimation;
        Vector2 stickPosition;

        int sticksMapping[4];
        u8 sticksInvert;
        float sticksMin[4];
        float sticksMax[4];

        void (*exitCallback)(GuiElement*);

        u8 sticksConfig[13];
        float angle;

    public:
        WizardMenu(TPLFile* tdf, Font* _font, int _width, int _height);

        ~WizardMenu() {
            delete leftStickBaseImg;
            delete leftStickImg;
            delete CStickBaseImg;
            delete CStickImg;
        }

        void draw(bool onFocus);
        void handleInputs();
        Vector2 getDimensions() {return Vector2(width, height);}

        void onActiveEvent() {
            state = LS_WAIT_FOR_MOVE;
            for (int i = 0; i < 4; i++) {
                sticksMapping[i] = i;
                sticksMin[i] = 127;
                sticksMax[i] = -128;
            }
            sticksInvert = 0;
            stickAnimation.reset();

            //Read configuration from GC+2.0
            GCPlus::unlock();
            GCPlus::readEEPROM(0x06, sticksConfig, 13);

            //Now write default configuration
            u8 tempConfig[13] = {0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x02, 0x03, 0x00, 0x01, 0x00};
            GCPlus::writeEEPROM(0x06, tempConfig, 13);
            GCPlus::reset();
        }

        void setExitCallback(void (*cb)(GuiElement*)) {
            exitCallback = cb;
        }
};

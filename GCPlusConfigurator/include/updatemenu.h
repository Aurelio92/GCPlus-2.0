#pragma once

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <libbbgui.h>
#include <ogc/lwp_threads.h>
#include <ogc/lwp_watchdog.h>
#include "gcplus.h"

class UpdateMenu : public GuiElement {
    private:
        Font* font;
        int textCenterOffsetY;

        void (*exitCallback)(GuiElement*);

        u16 oldVersion;
        u16 newVersion;

    public:
        UpdateMenu(Font* _font, int _width, int _height, u8* _payload, u32 _payloadSize);

        void draw(bool onFocus);
        void handleInputs();
        Vector2 getDimensions() {return Vector2(width, height);}

        void setExitCallback(void (*cb)(GuiElement*)) {
            exitCallback = cb;
        }

        void onActiveEvent();
};

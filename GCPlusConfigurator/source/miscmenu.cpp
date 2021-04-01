#include <gccore.h>
#include <wiiuse/wpad.h>
#include <stdio.h>
#include <stdlib.h>
#include <libbbgui.h>
#include <math.h>
#include "miscmenu.h"
#include "gcplus.h"

#define DZ_MODE_RADIAL          0
#define DZ_MODE_SCALEDRADIAL    1

#define TRIG_MODE_DIGITAL   0
#define TRIG_MODE_ANALOG    1

#define FAST_CHANGE_DELAY 500
#define FAST_CHANGE_PERIOD 30

void MiscMenu::draw(bool onFocus) {
    GFXWindow(32, 32, 0, 0) {
        if (font) {
            font->printf(12, 0 + 16 - textCenterOffsetY, "Deadzone radius: %u [0-60]", miscConfig[0]);
            font->printf(12, 32 + 16 - textCenterOffsetY, "Deadzone mode: %s",
                (miscConfig[2] == DZ_MODE_RADIAL) ? "Radial" : "Scaled radial");
            font->printf(12, 64 + 16 - textCenterOffsetY, "Triggers mode: %s", (miscConfig[4] == TRIG_MODE_DIGITAL) ? "Digital" : "Analog");
            font->printf(12, 96 + 16 - textCenterOffsetY, "Rumble intensity: %u [0-127]", miscConfig[3] - 128);
        }
        if (selected == 0)
            drawEmptyRectangle(6, selected * 32, font->getTextWidth("Deadzone radius: %u [0-60]", miscConfig[0]) + 12, 32, 2.0, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));
        else if (selected == 1)
            drawEmptyRectangle(6, selected * 32, font->getTextWidth("Deadzone mode: %s",
                (miscConfig[2] == DZ_MODE_RADIAL) ? "Radial" : "Scaled radial") + 12, 32, 2.0, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));
        else if (selected == 2)
            drawEmptyRectangle(6, selected * 32, font->getTextWidth("Triggers mode: %s", (miscConfig[4] == TRIG_MODE_DIGITAL) ? "Digital" : "Analog") + 12, 32, 2.0, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));
        else if (selected == 3)
            drawEmptyRectangle(6, selected * 32, font->getTextWidth("Rumble intensity: %u [0-127]", miscConfig[3] - 128) + 12, 32, 2.0, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));

        GFXWindow(0, 280, 0, 0) {
            font->printf(0, 0, "Press A to save settings");
            font->printf(0, 20, "Press B to go back");
        }
    }
}

void MiscMenu::handleInputs() {
    int down = PAD_ButtonsDown(0);
    int wDown = WPAD_ButtonsDown(0);
    int held = PAD_ButtonsHeld(0);
    int wHeld = WPAD_ButtonsHeld(0);
    static u64 lastPressTime = 0;

    if ((down & PAD_BUTTON_DOWN) || (wDown & WPAD_BUTTON_DOWN))
        selected++;

    if (((down & PAD_BUTTON_UP) || (wDown & WPAD_BUTTON_UP)) && selected > 0)
        selected--;

    if (selected > 3) {
        selected = 3;
    }

    //Save configurations and go back
    if (((down & PAD_BUTTON_A) || (wDown & WPAD_BUTTON_A)) && exitCallback) {
        GCPlus::unlock();
        miscConfig[1] = miscConfig[0]; //CDeadzone = SDeadzone
        GCPlus::writeEEPROM(0x13, miscConfig, 5);
        GCPlus::reset();
        exitCallback(this->parentElement);
    }

    //Discard and go back
    if (((down & PAD_BUTTON_B) || (wDown & WPAD_BUTTON_B)) && exitCallback) {
        exitCallback(this->parentElement);
    }

    if ((down & PAD_BUTTON_LEFT) || (wDown & WPAD_BUTTON_LEFT)) {
        lastPressTime = ticks_to_millisecs(gettime()) + FAST_CHANGE_DELAY;
        if ((selected == 0) && miscConfig[0] > 0) {
            miscConfig[0]--;
        }
        if (selected == 1) {
            miscConfig[2] ^= 1;
        }
        if (selected == 2) {
            miscConfig[4] ^= 1;
        }
        if ((selected == 3) && miscConfig[3] > 128) {
            miscConfig[3]--;
        }
    }

    if ((down & PAD_BUTTON_RIGHT) || (wDown & WPAD_BUTTON_RIGHT)) {
        lastPressTime = ticks_to_millisecs(gettime()) + FAST_CHANGE_DELAY;
        if ((selected == 0) && miscConfig[0] < 60) {
            miscConfig[0]++;
        }
        if (selected == 1) {
            miscConfig[2] ^= 1;
        }
        if (selected == 2) {
            miscConfig[4] ^= 1;
        }
        if ((selected == 3) && miscConfig[3] < 255) {
            miscConfig[3]++;
        }
    }

    if (((held & PAD_BUTTON_LEFT) || (wHeld & WPAD_BUTTON_LEFT)) && (ticks_to_millisecs(gettime()) - FAST_CHANGE_PERIOD > lastPressTime)) {
        lastPressTime = ticks_to_millisecs(gettime());
        if ((selected == 0) && miscConfig[0] > 0) {
            miscConfig[0]--;
        }
        if ((selected == 3) && miscConfig[3] > 128) {
            miscConfig[3]--;
        }
    }

    if (((held & PAD_BUTTON_RIGHT) || (wHeld & WPAD_BUTTON_RIGHT)) && (ticks_to_millisecs(gettime()) - FAST_CHANGE_PERIOD > lastPressTime)) {
        lastPressTime = ticks_to_millisecs(gettime());
        if ((selected == 0) && miscConfig[0] < 60) {
            miscConfig[0]++;
        }
        if ((selected == 3) && miscConfig[3] < 255) {
            miscConfig[3]++;
        }
    }
}

#include <gccore.h>
#include <wiiuse/wpad.h>
#include <stdio.h>
#include <stdlib.h>
#include <libbbgui.h>
#include <math.h>
#include "testmenu.h"
#include "gcplus.h"

/*Start: 165, 80
A: 259, 66
B: 228, 95
X: 308, 61
Y: 251, 39
Dpad: 92, 142
Stick: 44, 68
C-Stick: 217, 152*/

void TestMenu::draw(bool onFocus) {
    static float angle = 0;
    angle += 1.0f;
    GFXWindow((width - outline->getDimensions().x) / 2, 32, 0, 0) {
        outline->draw(true);
        GFXWindow(165, 80, 0, 0) {
            if (buttonsHeld & PAD_BUTTON_START)
                Start_pressed->draw(true);
            else
                Start_outline->draw(true);
        }
        GFXWindow(259, 66, 0, 0) {
            if (buttonsHeld & PAD_BUTTON_A)
                A_pressed->draw(true);
            else
                A_outline->draw(true);
        }

        GFXWindow(228, 95, 0, 0) {
            if (buttonsHeld & PAD_BUTTON_B)
                B_pressed->draw(true);
            else
                B_outline->draw(true);
        }

        GFXWindow(308, 61, 0, 0) {
            if (buttonsHeld & PAD_BUTTON_X)
                X_pressed->draw(true);
            else
                X_outline->draw(true);
        }

        GFXWindow(251, 39, 0, 0) {
            if (buttonsHeld & PAD_BUTTON_Y)
                Y_pressed->draw(true);
            else
                Y_outline->draw(true);
        }

        GFXWindow(92, 142, 0, 0) {
            Dpad_outline->draw(true);
            if ((buttonsHeld & PAD_BUTTON_UP) && (buttonsHeld & PAD_BUTTON_RIGHT) &&
                (buttonsHeld & PAD_BUTTON_DOWN) && (buttonsHeld & PAD_BUTTON_LEFT)) {
                Dpad_full->draw(true);
            } else {
                if (buttonsHeld & PAD_BUTTON_UP) {
                    Dpad_pressed->draw(true);
                }
                if (buttonsHeld & PAD_BUTTON_RIGHT) {
                    Gfx::pushMatrix();
                    Gfx::rotate(Dpad_pressed->getDimensions().x / 2, Dpad_pressed->getDimensions().y / 2, 90);
                    Dpad_pressed->draw(true);
                    Gfx::popMatrix();
                }
                if (buttonsHeld & PAD_BUTTON_DOWN) {
                    Gfx::pushMatrix();
                    Gfx::rotate(Dpad_pressed->getDimensions().x / 2, Dpad_pressed->getDimensions().y / 2, 180);
                    Dpad_pressed->draw(true);
                    Gfx::popMatrix();
                }
                if (buttonsHeld & PAD_BUTTON_LEFT) {
                    Gfx::pushMatrix();
                    Gfx::rotate(Dpad_pressed->getDimensions().x / 2, Dpad_pressed->getDimensions().y / 2, 270);
                    Dpad_pressed->draw(true);
                    Gfx::popMatrix();
                }
            }

        }

        GFXWindow(34, 1, 0, 0) {
            if (buttonsHeld & PAD_TRIGGER_L)
                L_pressed->draw(true);
            else if (la > 0)
                L_analog->draw(true);
            else
                L_outline->draw(true);

            if (la > 0)
                fontSmall->printf(-16, 0, "%.0f", la);
        }

        GFXWindow(259, 1, 0, 0) {
            if (buttonsHeld & PAD_TRIGGER_R)
                R_pressed->draw(true);
            else if (ra > 0)
                R_analog->draw(true);
            else
                R_outline->draw(true);

            if (ra > 0)
                fontSmall->printf(64, 0, "%.0f", ra);
        }

        GFXWindow(264, 9, 0, 0) {
            if (buttonsHeld & PAD_TRIGGER_Z)
                Z_pressed->draw(true);
            else
                Z_outline->draw(true);
        }

        GFXWindow(42 + sx * 20 / 128, 68 - sy * 20 / 128, 0, 0) {
            Stick_pressed->draw(true);
        }

        GFXWindow(217 + cx * 20 / 128, 154 - cy * 20 / 128, 0, 0) {
            CStick_pressed->draw(true);
        }

        GFXWindow(0, 280, 0, 0) {
            font->printf(0, 0, "Hold A+B to activate rumble");
            font->printf(0, 20, "Press L+R to go back");
        }
    }
}

void TestMenu::handleInputs() {
    buttonsHeld = PAD_ButtonsHeld(0);
    sx = PAD_StickX(0);
    sy = PAD_StickY(0);
    cx = PAD_SubStickX(0);
    cy = PAD_SubStickY(0);
    ra = PAD_TriggerR(0);
    la = PAD_TriggerL(0);

    if ((buttonsHeld & PAD_BUTTON_A) && (buttonsHeld & PAD_BUTTON_B)) {
        //Rumble
        PAD_ControlMotor(0, 1);
    } else {
        PAD_ControlMotor(0, 0);
    }

    if ((buttonsHeld & PAD_TRIGGER_L) && (buttonsHeld & PAD_TRIGGER_R) && exitCallback) {
        exitCallback(this->parentElement);
    }
}

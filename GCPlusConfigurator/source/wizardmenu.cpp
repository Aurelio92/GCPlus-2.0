#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <libbbgui.h>
#include <math.h>
#include "wizardmenu.h"
#include "gcplus.h"
#include "log.h"

#define PI 3.14159265359f
#define STICKS_THRESHOLD 30

WizardMenu::WizardMenu(TPLFile* tdf, Font* _font, int _width, int _height) {
    font = _font;
    width = _width;
    height = _height;
    state = LS_WAIT_FOR_MOVE;

    //Load sticks images
    leftStickBaseImg = new GuiImage(tdf, TEXID_LeftStick_base);
    leftStickBaseImg->setSize(128, 128);
    leftStickImg = new GuiImage(tdf, TEXID_LeftStick);
    leftStickImg->setSize(128, 128);
    CStickBaseImg = new GuiImage(tdf, TEXID_CStick_base);
    CStickBaseImg->setSize(128, 128);
    CStickImg = new GuiImage(tdf, TEXID_CStick);
    CStickImg->setSize(128, 128);

    //Build animation
    Vector2 xy1;
    Vector2 xy2;
    stickAnimation.setOutput(&stickPosition);
    xy1 = Vector2(0, 0);
    xy2 = Vector2(0, 0);
    stickAnimation.addStep(millisecs_to_ticks(1000), xy1, xy2);
    xy1 = Vector2(0, 0);
    xy2 = Vector2(48, 0);
    stickAnimation.addStep(millisecs_to_ticks(100), xy1, xy2);
    xy1 = Vector2(48, 0);
    xy2 = Vector2(48, 0);
    stickAnimation.addStep(millisecs_to_ticks(1000), xy1, xy2);

    for (int i = 0; i < 99; i++) {
        float x1 = 48 * cos(i * 2 * PI / 100);
        float y1 = -48 * sin(i * 2 * PI / 100);
        float x2 = 48 * cos((i + 1) * 2 * PI / 100);
        float y2 = -48 * sin((i + 1) * 2 * PI / 100);
        xy1 = Vector2(x1, y1);
        xy2 = Vector2(x2, y2);
        stickAnimation.addStep(millisecs_to_ticks(10), xy1, xy2);
    }
    xy1 = Vector2(48, 0);
    xy2 = Vector2(48, 0);
    stickAnimation.addStep(millisecs_to_ticks(300), xy1, xy2);
    stickAnimation.addReturnToHomeStep(millisecs_to_ticks(100));
    stickAnimation.resume();

    for (int i = 0; i < 4; i++) {
        sticksMapping[i] = i;
        sticksMin[i] = 127;
        sticksMax[i] = -128;
    }
    sticksInvert = 0;

    exitCallback = NULL;
}

void WizardMenu::draw(bool onFocus) {
    float sticks[4];

    sticks[0] = PAD_StickX(0);
    sticks[1] = PAD_StickY(0);
    sticks[2] = PAD_SubStickX(0);
    sticks[3] = PAD_SubStickY(0);

    //Update stick animation and draw it
    stickAnimation.animate();
    GFXWindow(width / 2 - leftStickBaseImg->getDimensions().x - 64, 64, 0, 0) {
        leftStickBaseImg->draw();
        if (state <= LS_WAIT_FOR_RELEASE) {
            GFXWindow(stickPosition.x, stickPosition.y, 0, 0) {
                leftStickImg->draw();
            }
        } else if (state < STICKS_TEST) {
            leftStickImg->draw();
        } else {
            GFXWindow(48 * sticks[0] / 127, -48 * sticks[1] / 127, 0, 0) {
                leftStickImg->draw();
            }
        }
    }
    GFXWindow(width / 2 + 64, 64, 0, 0) {
        CStickBaseImg->draw();
        if (state < CS_WAIT_FOR_MOVE) {
            CStickImg->draw();
        } else if (state < STICKS_TEST) {
            GFXWindow(stickPosition.x, stickPosition.y, 0, 0) {
                CStickImg->draw();
            }
        } else {
            GFXWindow(48 * sticks[2] / 127, -48 * sticks[3] / 127, 0, 0) {
                CStickImg->draw();
            }
        }
    }
    GFXWindow(16, 256, 0, 0) {
        switch(state) {
            case LS_WAIT_FOR_MOVE:
                font->printf(0, 0, "Move the left stick to the far right.");
                font->printf(0, 20, "Press B to cancel.");
            break;

            case LS_WAIT_FOR_ROT1:
                font->printf(0, 0, "Move the left stick in anti-clockwise direction.");
                font->printf(0, 20, "Press B to cancel.");
            break;

            case LS_WAIT_FOR_ROT2:
            case LS_WAIT_FOR_ROT3:
            case LS_WAIT_FOR_ROT4:
                font->printf(0, 0, "Keep turning");
                font->printf(0, 20, "Press B to cancel.");
            break;

            case LS_WAIT_FOR_RELEASE:
                font->printf(0, 0, "Release the left stick.");
                font->printf(0, 20, "Press B to cancel.");
            break;

            case CS_WAIT_FOR_MOVE:
                font->printf(0, 0, "Move the C-stick to the far right.");
                font->printf(0, 20, "Press B to cancel.");
            break;

            case CS_WAIT_FOR_ROT1:
                font->printf(0, 0, "Move the C-stick in anti-clockwise direction.");
                font->printf(0, 20, "Press B to cancel.");
            break;

            case CS_WAIT_FOR_ROT2:
            case CS_WAIT_FOR_ROT3:
            case CS_WAIT_FOR_ROT4:
                font->printf(0, 0, "Keep turning.");
                font->printf(0, 20, "Press B to cancel.");
            break;

            case CS_WAIT_FOR_RELEASE:
                font->printf(0, 0, "Release the C-stick.");
                font->printf(0, 20, "Press B to cancel.");
            break;

            case STICKS_TEST:
                font->printf(0, 0, "Test your sticks and make sure they work properly.");
                font->printf(0, 20, "Press A to save or B to cancel.");
            break;
        }
        //font->printf(0, 40, "%d [%d %d %d %d] %f", sticksInvert, sticksMapping[0], sticksMapping[1], sticksMapping[2], sticksMapping[3], angle);
        //for (int i = 0; i < 2; i++)
            //font->printf(0, 60 + i * 20, "%f %f %f %f", sticksMin[sticksMapping[2*i]], sticksMax[sticksMapping[2*i]], sticksMin[sticksMapping[2*i+1]], sticksMax[sticksMapping[2*i+1]]);
    }
}

void WizardMenu::handleInputs() {
    float sticks[4];
    float maxStick;
    int maxIdx;
    //float angle;
    int down;

    sticks[0] = PAD_StickX(0);
    sticks[1] = PAD_StickY(0);
    sticks[2] = PAD_SubStickX(0);
    sticks[3] = PAD_SubStickY(0);
    down = PAD_ButtonsDown(0);

    if (down & PAD_BUTTON_B && exitCallback) {
        //Write previous config and go back
        GCPlus::unlock();
        GCPlus::writeEEPROM(0x06, sticksConfig, 13);
        GCPlus::reset();
        exitCallback(this->parentElement);
    }

    //Check ranges
    for (int i = 0; i < 4; i++) {
        if (sticks[i] > sticksMax[i])
            sticksMax[i] = sticks[i];
        if (sticks[i] < sticksMin[i])
            sticksMin[i] = sticks[i];
    }

    //Check which stick has the max absoulute value
    maxStick = sticks[0];
    maxIdx = 0;
    for (int i = 1; i < 4; i++) {
        if (abs(sticks[i]) > abs(maxStick)) {
            maxStick = sticks[i];
            maxIdx = i;
        }
    }

    switch (state) {
        case LS_WAIT_FOR_MOVE:
            //Check if a stick has moved
            if (abs(maxStick) > STICKS_THRESHOLD) {
                sticksMapping[0] = maxIdx;
                if (maxStick < 0) {
                    sticksInvert |= 1;
                }
                state = LS_WAIT_FOR_ROT1;
            }
        break;

        case LS_WAIT_FOR_ROT1:
            //Check if the stick has been rotated
            angle = atan2(maxStick, (sticksInvert & 1) ? -sticks[sticksMapping[0]] : sticks[sticksMapping[0]]);
            if (abs(angle) > PI / 3.0 && maxIdx != sticksMapping[0]) {
                sticksMapping[1] = maxIdx;
                if (angle < 0) {
                    sticksInvert |= 2;
                }
                state = LS_WAIT_FOR_ROT2;
            }
        break;

        case LS_WAIT_FOR_ROT2:
            //Wait until the stick is on the left
            angle = atan2((sticksInvert & 2) ? -sticks[sticksMapping[1]] : sticks[sticksMapping[1]], (sticksInvert & 1) ? -sticks[sticksMapping[0]] : sticks[sticksMapping[0]]);
            if (abs(angle) > 5 * PI / 6) {
                state = LS_WAIT_FOR_ROT3;
            }
        break;

        case LS_WAIT_FOR_ROT3:
            //Wait until the stick is on the bottom left
            angle = atan2((sticksInvert & 2) ? -sticks[sticksMapping[1]] : sticks[sticksMapping[1]], (sticksInvert & 1) ? -sticks[sticksMapping[0]] : sticks[sticksMapping[0]]);
            if (angle < -PI / 3.0) {
                state = LS_WAIT_FOR_ROT4;
            }
        break;

        case LS_WAIT_FOR_ROT4:
            //Wait until the stick is on the right
            angle = atan2((sticksInvert & 2) ? -sticks[sticksMapping[1]] : sticks[sticksMapping[1]], (sticksInvert & 1) ? -sticks[sticksMapping[0]] : sticks[sticksMapping[0]]);
            if (abs(angle) < 0.1) {
                state = LS_WAIT_FOR_RELEASE;
            }
        break;

        case LS_WAIT_FOR_RELEASE:
            //Wait for the stick to be released
            if (sqrt(sticks[sticksMapping[0]] * sticks[sticksMapping[0]] + sticks[sticksMapping[1]] * sticks[sticksMapping[1]]) < STICKS_THRESHOLD) {
                state = CS_WAIT_FOR_MOVE;
                stickAnimation.reset();
            }
        break;

        case CS_WAIT_FOR_MOVE:
            //Check if a stick has moved
            if (abs(maxStick) > STICKS_THRESHOLD) {
                sticksMapping[2] = maxIdx;
                if (maxStick < 0) {
                    sticksInvert |= 4;
                }
                state = CS_WAIT_FOR_ROT1;
            }
        break;

        case CS_WAIT_FOR_ROT1:
            //Check if the stick has been rotated
            angle = atan2(maxStick, (sticksInvert & 4) ? -sticks[sticksMapping[2]] : sticks[sticksMapping[2]]);
            if (abs(angle) > PI / 3.0  && maxIdx != sticksMapping[2]) {
                sticksMapping[3] = maxIdx;
                if (angle < 0) {
                    sticksInvert |= 8;
                }
                state = CS_WAIT_FOR_ROT2;
            }
        break;

        case CS_WAIT_FOR_ROT2:
            //Wait until the stick is on the left
            angle = atan2((sticksInvert & 8) ? -sticks[sticksMapping[3]] : sticks[sticksMapping[3]], (sticksInvert & 4) ? -sticks[sticksMapping[2]] : sticks[sticksMapping[2]]);
            if (abs(angle) > 5 * PI / 6) {
                state = CS_WAIT_FOR_ROT3;
            }
        break;

        case CS_WAIT_FOR_ROT3:
            //Wait until the stick is on the bottom left
            angle = atan2((sticksInvert & 8) ? -sticks[sticksMapping[3]] : sticks[sticksMapping[3]], (sticksInvert & 4) ? -sticks[sticksMapping[2]] : sticks[sticksMapping[2]]);
            if (angle < -PI / 3.0) {
                state = CS_WAIT_FOR_ROT4;
            }
        break;

        case CS_WAIT_FOR_ROT4:
            //Wait until the stick is on the right
            angle = atan2((sticksInvert & 8) ? -sticks[sticksMapping[3]] : sticks[sticksMapping[3]], (sticksInvert & 4) ? -sticks[sticksMapping[2]] : sticks[sticksMapping[2]]);
            if (abs(angle) < 0.1) {
                state = CS_WAIT_FOR_RELEASE;
            }
        break;

        case CS_WAIT_FOR_RELEASE:
            //Wait for the stick to be released
            if (sqrt(sticks[sticksMapping[2]] * sticks[sticksMapping[2]] + sticks[sticksMapping[3]] * sticks[sticksMapping[3]]) < STICKS_THRESHOLD) {
                state = STICKS_TEST;

                //Write new config to GC+2.0
                u8 tempConfig[13];
                u8 chan[4] = {0x02, 0x03, 0x00, 0x01};
                for (int i = 0; i < 4; i++) {
                    float temp;
                    if (sticksInvert & (1 << i))
                        temp = -sticksMax[sticksMapping[i]] + 128;
                    else
                        temp = sticksMin[sticksMapping[i]] + 128;
                    if (temp < 0)
                        temp = 0;
                    if (temp > 255)
                        temp = 255;
                    tempConfig[2 * i] = (u8)round(temp);
                    if (sticksInvert & (1 << i))
                        temp = -sticksMin[sticksMapping[i]] + 128;
                    else
                        temp = sticksMax[sticksMapping[i]] + 128;
                    if (temp < 0)
                        temp = 0;
                    if (temp > 255)
                        temp = 255;
                    tempConfig[2 * i + 1] = (u8)round(temp);

                    tempConfig[8 + i] = chan[sticksMapping[i]];
                }
                tempConfig[12] = sticksInvert;

                GCPlus::unlock();
                GCPlus::writeEEPROM(0x06, tempConfig, 13);
                GCPlus::reset();
            }
        break;

        case STICKS_TEST:
            if (down & PAD_BUTTON_A && exitCallback) {
                //Configuration has already been written, we just need to go back
                exitCallback(this->parentElement);
            }
        break;
    }
}

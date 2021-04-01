#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <libbbgui.h>
#include <ogc/lwp_watchdog.h>
#include <math.h>
#include <wiiuse/wpad.h>
#include <unistd.h>
#include "updatemenu.h"
#include "main.h"

#define PROGRESSBAR_WIDTH 200
#define PROGRESSBAR_HEIGHT 16

#define STACKSIZE 8192

static u8* payload;
static u32 payloadSize;
static lwp_t threadHandle;
static u8* threadStack;
static mutex_t updateMutex;
static float progress;
static bool doneUpdating;
static u8 EEPROMConf[24];

static void* updateThread(void* arg) {
    GCPlus::flashPayload(payload, payloadSize, updateMutex, &progress);
    u64 now = gettime();
    while (ticks_to_millisecs(gettime() - now) < 1000);
    if (!GCPlus::bootPayload())
        return NULL;
    now = gettime();
    while (ticks_to_millisecs(gettime() - now) < 1000);
    //When switching mode we must unlock again
    if (!GCPlus::unlock())
        return NULL;
    if (!GCPlus::getFWVer((u16*)arg))
        return NULL;
    #ifdef DEMOBUILD
    *(u16*)arg = (2 << 3) | 1;
    #endif

    /*if (!GCPlus::writeEEPROM(0, EEPROMConf, 12))
        return NULL;
    if (!GCPlus::writeEEPROM(12, &EEPROMConf[12], 12))
        return NULL;*/

    if (!GCPlus::reset()) //reset and lock
        return NULL;
    LWP_MutexLock(updateMutex);
    doneUpdating = true;
    LWP_MutexUnlock(updateMutex);
    return NULL;
}

UpdateMenu::UpdateMenu(Font* _font, int _width, int _height, u8* _payload, u32 _payloadSize) {
    font = _font;
    width = _width;
    height = _height;
    payload = _payload;
    payloadSize = _payloadSize;

    if (font)
        textCenterOffsetY = font->getSize() - font->getCharHeight('O') / 2;

    LWP_MutexInit(&updateMutex, false);

    threadStack = (u8 *)memalign(32, STACKSIZE);
}

void UpdateMenu::draw(bool onFocus) {
    GFXWindow(32, 32, 0, 0) {
        LWP_MutexLock(updateMutex);
        if (!payloadSize) {
            font->printf(0, 0, "Update file missing\n");
            font->printf(0, 20, "Press any button to go back\n");
        } else {
            drawEmptyRectangle(0, 16 - textCenterOffsetY, PROGRESSBAR_WIDTH, PROGRESSBAR_HEIGHT, 2.0, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));
            drawRectangle(0, 16 - textCenterOffsetY, progress * PROGRESSBAR_WIDTH / 100.0f, PROGRESSBAR_HEIGHT, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));
            if (font) {
                font->printf(PROGRESSBAR_WIDTH + 16, 0, "%.1f%%", progress);
                font->printf(0, 20, "Old version: %u.%u", (oldVersion >> 3), oldVersion & 0x0007);
                if (doneUpdating) {
                    font->printf(0, 40, "Update complete. New version: %u.%u", (newVersion >> 3), newVersion & 0x0007);
                    font->printf(0, 60, "Press any button to go back\n");
                }
            }
        }
        LWP_MutexUnlock(updateMutex);
    }
}

void UpdateMenu::handleInputs() {
    int down = PAD_ButtonsDown(0);
    int wdown = WPAD_ButtonsDown(0);

    LWP_MutexLock(updateMutex);
    if ((down || wdown) && exitCallback && doneUpdating) {
        exitCallback(this->parentElement);
    }
    LWP_MutexUnlock(updateMutex);
}

void UpdateMenu::onActiveEvent() {
    //Don't update if payload file is missing
    if (!payloadSize) {
        doneUpdating = true;
        return;
    }

    progress = 0.0f;
    doneUpdating = false;
    oldVersion = 0;
    newVersion = 0;
    u64 now;
    if (!GCPlus::unlock())
        return;

    if (!GCPlus::getFWVer(&oldVersion))
        return;

    /*if (!GCPlus::readEEPROM(0, EEPROMConf, 12))
        return;
    if (!GCPlus::readEEPROM(12, &EEPROMConf[12], 12))
        return;*/

    #ifdef DEMOBUILD
    oldVersion = (2 << 3);
    #endif

    if (!GCPlus::bootBootloader())
        return;

    now = gettime();
    while (ticks_to_millisecs(gettime() - now) < 1000);
    //When switching mode we must unlock again
    if (!GCPlus::unlock())
        return;

    LWP_CreateThread(&threadHandle, updateThread, &newVersion, threadStack, STACKSIZE, 30);
}

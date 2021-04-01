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

#include <libbbgui.h>

#include "Menlo-Regular_ttf.h"
#include "SFMono-Regular_otf.h"
#include "FSEX300_ttf.h"

#include "textures.h"
#include "textures_tpl.h"
#include "textures_pressed.h"
#include "textures_pressed_tpl.h"

#include "gcplus.h"
#include "menu.h"
#include "wizardmenu.h"
#include "miscmenu.h"
#include "testmenu.h"
#include "updatemenu.h"
#include "log.h"
#include "hex.h"

#define TOPBAR_HEIGHT 68
#define BOTTOMBAR_HEIGHT 44

class ExitElement : public GuiElement {
    public:
        void onActiveEvent() {
            exit(0);
        }
};

GuiWindow* masterWindow;
GuiWindow* mainWindow;
GuiWindow* wizardWindow;
GuiWindow* miscWindow;
GuiWindow* testWindow;
GuiWindow* updateWindow;
ExitElement* exitElement;

static mutex_t SIMutex;

void lockSIMutex() {
    LWP_MutexLock(SIMutex);
}

void unlockSIMutex() {
    LWP_MutexUnlock(SIMutex);
}

void switchBackToMainMenu(GuiElement* caller) {
    masterWindow->setElementActive(caller, false);
    masterWindow->focusOnElement(mainWindow);
    masterWindow->setElementActive(mainWindow, true);
    VIDEO_WaitVSync();
    PAD_ScanPads();
    VIDEO_WaitVSync();
    PAD_ScanPads();
    VIDEO_WaitVSync();
}

void switchToMenu(GuiElement* win) {
    masterWindow->setElementActive(mainWindow, false);
    masterWindow->focusOnElement(win);
    masterWindow->setElementActive(win, true);
    VIDEO_WaitVSync();
    PAD_ScanPads();
    VIDEO_WaitVSync();
    PAD_ScanPads();
    VIDEO_WaitVSync();
}

int main(int argc, char** argv) {
    TPLFile tdf;
    TPLFile tdf_pressed;
    Vector2 screenSize;
    bool didTimeout = false;

    Gfx::init();
    Gfx::setClearColor(0x30, 0x30, 0x30);
    PAD_Init();
    WPAD_Init();
    fatInitDefault();

    LWP_MutexInit(&SIMutex, true);

    IntelHex hex("/GCPlus2.0Update.hex");

    #ifdef DEMOBUILD
    hex.binary = (u8*)malloc(1024 * 8);
    hex.binarySize = 1024 * 8;
    #endif

    Font sfmono(SFMono_Regular_otf, SFMono_Regular_otf_size, 20);
    Font sfmonoSmall(SFMono_Regular_otf, SFMono_Regular_otf_size, 12);

    TPL_OpenTPLFromMemory(&tdf, (void*)textures_tpl, textures_tpl_size);
    TPL_OpenTPLFromMemory(&tdf_pressed, (void*)textures_pressed_tpl, textures_pressed_tpl_size);

    if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
        screenSize.x = 800;
        screenSize.y = 480;
    } else {
        screenSize.x = 640;
        screenSize.y = 480;
    }

    masterWindow = new GuiWindow(screenSize.x, screenSize.y);

    //Top bar
    GuiWindow topBar(screenSize.x, TOPBAR_HEIGHT);
    topBar.setColor(RGBA8(0x40, 0x40, 0x40, 0xFF));
    GuiLabel topLabel(&sfmono, L"GC+2.0 configurator");
    topBar.addElement(&topLabel, 12, 28);
    masterWindow->addElement(&topBar, 0, 0);

    //Bottom bar
    GuiWindow bottomBar(screenSize.x, BOTTOMBAR_HEIGHT);
    bottomBar.setColor(RGBA8(0x40, 0x40, 0x40, 0xFF));
    GuiLabel bottomLabel(&sfmono, L"v1.1");
    bottomBar.addElement(&bottomLabel, 12, 8);
    masterWindow->addElement(&bottomBar, 0, screenSize.y - BOTTOMBAR_HEIGHT);

    #ifndef DEMOBUILD
    //Wait until a controller is connected for at least 500 ms
    {
        u32 connected;
        u64 now = gettime();
        u64 timeout = gettime();
        do {
            connected = PAD_ScanPads();
            if (!(connected & 1)) {
                now = gettime();
            }
            if (ticks_to_millisecs(gettime() - timeout) > 3000) {
                didTimeout = true;
                break;
            }
            VIDEO_WaitVSync();
        } while (ticks_to_millisecs(gettime() - now) < 500);
    }

    if (didTimeout) {
        u64 now = gettime();
        while(1) {
            GFXDraw {
                sfmono.printf(40, 20, "GC+2.0 is required to run this homebrew");
                sfmono.printf(40, 40, "Will now exit");
            }

            if (ticks_to_millisecs(gettime() - now) > 2000)
                exit(0);
        }
    }

    if (GCPlus::isV1()) {
        u64 now = gettime();
        while(1) {
            GFXDraw {
                sfmono.printf(40, 20, "GC+ 1.0 is not supported by this homebrew");
                sfmono.printf(40, 40, "Will now exit");
            }

            if (ticks_to_millisecs(gettime() - now) > 2000)
                exit(0);
        }
    }

    if (!GCPlus::isV2()) {
        u64 now = gettime();
        while(1) {
            GFXDraw {
                sfmono.printf(40, 20, "GC+2.0 is required to run this homebrew");
                sfmono.printf(40, 40, "Will now exit");
            }

            if (ticks_to_millisecs(gettime() - now) > 2000)
                exit(0);
        }
    }
    #endif

    //Main menu
    mainWindow = new GuiWindow(screenSize.x, screenSize.y - (TOPBAR_HEIGHT + BOTTOMBAR_HEIGHT));
    masterWindow->addElement(mainWindow, 0, TOPBAR_HEIGHT);

    Menu menu(&sfmono, screenSize.x, screenSize.y - (TOPBAR_HEIGHT + BOTTOMBAR_HEIGHT));
    menu.setSwitchToMenu(switchToMenu);
    mainWindow->addElement(&menu, 0, 0);
    mainWindow->focusOnElement(&menu);

    //Wizard menu
    wizardWindow = new GuiWindow(screenSize.x, screenSize.y - (TOPBAR_HEIGHT + BOTTOMBAR_HEIGHT));
    masterWindow->addElement(wizardWindow, 0, TOPBAR_HEIGHT, false);

    WizardMenu wizardMenu(&tdf, &sfmono, screenSize.x, screenSize.y - (TOPBAR_HEIGHT + BOTTOMBAR_HEIGHT));
    wizardMenu.setExitCallback(switchBackToMainMenu);
    wizardWindow->addElement(&wizardMenu, 0, 0);
    wizardWindow->focusOnElement(&wizardMenu);

    //Misc menu
    miscWindow = new GuiWindow(screenSize.x, screenSize.y - (TOPBAR_HEIGHT + BOTTOMBAR_HEIGHT));
    masterWindow->addElement(miscWindow, 0, TOPBAR_HEIGHT, false);

    MiscMenu miscMenu(&tdf, &sfmono, screenSize.x, screenSize.y - (TOPBAR_HEIGHT + BOTTOMBAR_HEIGHT));
    miscMenu.setExitCallback(switchBackToMainMenu);
    miscWindow->addElement(&miscMenu, 0, 0);
    miscWindow->focusOnElement(&miscMenu);

    //Misc menu
    testWindow = new GuiWindow(screenSize.x, screenSize.y - (TOPBAR_HEIGHT + BOTTOMBAR_HEIGHT));
    masterWindow->addElement(testWindow, 0, TOPBAR_HEIGHT, false);

    TestMenu testMenu(&tdf, &tdf_pressed, &sfmono, &sfmonoSmall, screenSize.x, screenSize.y - (TOPBAR_HEIGHT + BOTTOMBAR_HEIGHT));
    testMenu.setExitCallback(switchBackToMainMenu);
    testWindow->addElement(&testMenu, 0, 0);
    testWindow->focusOnElement(&testMenu);

    //Update menu
    updateWindow = new GuiWindow(screenSize.x, screenSize.y - (TOPBAR_HEIGHT + BOTTOMBAR_HEIGHT));
    masterWindow->addElement(updateWindow, 0, TOPBAR_HEIGHT, false);

    UpdateMenu updateMenu(&sfmono, screenSize.x, screenSize.y - (TOPBAR_HEIGHT + BOTTOMBAR_HEIGHT), hex.binary, hex.binarySize);
    updateMenu.setExitCallback(switchBackToMainMenu);
    updateWindow->addElement(&updateMenu, 0, 0);
    updateWindow->focusOnElement(&updateMenu);

    //Exit element
    exitElement = new ExitElement();
    masterWindow->addElement(exitElement, 0, TOPBAR_HEIGHT, false);

    menu.addSubMenu(wizardWindow, "Sticks wizard");
    menu.addSubMenu(miscWindow, "Misc settings");
    /*menu.addSubMenu(NULL, "Triggers settings");
    menu.addSubMenu(NULL, "Rumble settings");*/
    menu.addSubMenu(testWindow, "Buttons test");
    menu.addSubMenu(updateWindow, "Update");
    menu.addSubMenu(exitElement, "Exit");

    masterWindow->focusOnElement(mainWindow);
    masterWindow->setElementActive(mainWindow, true);

    while (1) {
        lockSIMutex();
        PAD_ScanPads();
        unlockSIMutex();
        WPAD_ScanPads();

        masterWindow->handleInputs();

        GFXDraw {
            masterWindow->draw(true);
        }
    }

    return 0;

}

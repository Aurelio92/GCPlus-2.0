#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <libbbgui.h>
#include <wiiuse/wpad.h>
#include "menu.h"
#include "log.h"

void Menu::draw(bool onFocus) {
    GFXWindow(32, 32, 0, 0) {
        if (font) {
            int y = 0;
            for (auto& s : subMenus) {
                font->printf(12, y + 16 - textCenterOffsetY, s.text.c_str());
                y += 32;
            }
        }
        drawEmptyRectangle(6, selected * 32, font->getTextWidth(subMenus[selected].text.c_str()) + 12, 32, 2.0, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));
    }
}

void Menu::handleInputs() {
    int down = PAD_ButtonsDown(0);
    int wdown = WPAD_ButtonsDown(0);

    if ((down & PAD_BUTTON_DOWN) || (wdown & WPAD_BUTTON_DOWN))
        selected++;

    if (((down & PAD_BUTTON_UP) || (wdown & WPAD_BUTTON_UP)) && selected > 0)
        selected--;

    if (selected >= subMenus.size()) {
        selected = subMenus.size() - 1;
    }

    if (((down & PAD_BUTTON_A) || (wdown & WPAD_BUTTON_A)) && switchToMenu && subMenus[selected].element) {
        switchToMenu(subMenus[selected].element);
    }
}

void Menu::addSubMenu(GuiElement* element, const char* text) {
    subMenus.push_back(SubMenu(element, text));
}

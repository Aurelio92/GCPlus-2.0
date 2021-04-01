#pragma once

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <libbbgui.h>
#include <vector>
#include "log.h"

class Menu : public GuiElement {
    private:
        struct SubMenu {
                GuiElement* element;
                std::string text;

                SubMenu(GuiElement* element, std::string text) : element(element), text(text) {}
                SubMenu() {}
        };

        Font* font;
        u32 selected;
        int textCenterOffsetY;

        std::vector<SubMenu> subMenus;

        void (*switchToMenu)(GuiElement*);

    public:
        Menu(Font* _font, int _width, int _height) {
            font = _font;
            width = _width;
            height = _height;
            selected = 0;

            if (font)
                textCenterOffsetY = font->getSize() - font->getCharHeight('O') / 2;
        }

        ~Menu() {}

        void draw(bool onFocus);
        void handleInputs();
        Vector2 getDimensions() {return Vector2(width, height);}

        void addSubMenu(GuiElement* element, const char* text);

        void setSwitchToMenu(void (*cb)(GuiElement*)) {
            switchToMenu = cb;
        }
};

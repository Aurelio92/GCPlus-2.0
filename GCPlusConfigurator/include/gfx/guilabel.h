#pragma once

#include "gui.h"
#include <string>

class GuiLabel : public GuiElement {
    private:
        std::wstring text;
        Font* font;
    public:
        GuiLabel();
        GuiLabel(Font* _font, std::wstring _text);
        void draw();
        Rect getRect();

        //GuiImage& operator = (const GuiImage& img);
};
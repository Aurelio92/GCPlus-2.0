#include "gui.h"

GuiLabel::GuiLabel() {
	font = NULL;
}

GuiLabel::GuiLabel(Font* _font, std::wstring _text) {
    font = _font;
    text = _text;
}

void GuiLabel::draw() {
    font->printf(0, 0, text.c_str());
}

Rect GuiLabel::getRect() {
    Rect rect;
    rect.width = font->getTextWidth(text.c_str());
    rect.height = font->getSize();
    return rect;
}
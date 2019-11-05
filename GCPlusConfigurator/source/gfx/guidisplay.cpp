#include "guidisplay.h"

void GuiDisplay::addElement(GuiElement* el, int posX, int posY) {
    elements.push_back(ElementContainer(el, posX, posY));
}

void GuiDisplay::draw()
{
    for (auto& it : elements) { //Reference to avoid construction/destruction
        it.element->draw();
    }
}
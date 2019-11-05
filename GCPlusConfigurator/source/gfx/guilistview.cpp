#include "gui.h"
#include "gfx.h"

GuiListView::~GuiListView() {
    elements.clear();
}

void GuiListView::draw()
{
    Gfx::pushMatrix();
    for (auto& it : elements) {
        Rect elRect = it->getRect();
        Gfx::pushScissorBox(elRect.width, elRect.height);
        it->draw();
        Gfx::translate(0, elRect.height);
        Gfx::popScissorBox();
    }
    Gfx::popMatrix();
}

void GuiListView::addElement(GuiElement* el) {
    elements.push_back(el);
}

GuiListView& GuiListView::operator = (const GuiListView& l) {
    if (this == &l) { //Copying itself?
        return *this;
    }

    this->elements = l.elements;

    return *this;
}
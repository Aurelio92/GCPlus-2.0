#pragma once

#include <list>
#include "gui.h"

class GuiListView : public GuiElement {
    private:
        std::list<GuiElement*> elements;
    public:
        GuiListView() {}
        ~GuiListView();
        void draw();

        void addElement(GuiElement* el);

        GuiListView& operator = (const GuiListView& l);
};
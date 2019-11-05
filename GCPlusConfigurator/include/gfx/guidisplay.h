#pragma once

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include "gui.h"

class GuiDisplay {
    private:
        struct ElementContainer {
                GuiElement* element;
                int posX, posY;

                ElementContainer(GuiElement* element, int posX, int posY) : element(element), posX(posX), posY(posY) {}
                ElementContainer() {}
        };

        std::list<ElementContainer> elements;

    public:
        GuiDisplay() {};

        void addElement(GuiElement* el, int posX, int posY);
        void handleInputs();
        void draw();
};

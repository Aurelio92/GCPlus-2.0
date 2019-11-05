#pragma once

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include "guirect.h"

class GuiElement {
    protected:
        int width;
        int height;

    private:
        friend class GuiDisplay;

    public:
        GuiElement() {};
        //~GuiElement();

        virtual void draw() {}
        virtual void handleInputs() {} // Inherited classes override this
        virtual Rect getRect() {return Rect(0, 0, 0, 0);}
};

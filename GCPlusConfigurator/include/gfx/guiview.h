#pragma once

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include "gui.h"

class GuiView : public GuiElement {
    /*private:
        friend class Gui;


    protected:
        gfxScreen_t screen;
        Vec2 dimensions;*/

    public:
        //GuiElement();
        //~GuiElement();

        virtual void draw() {}
        virtual int handleInputs(int down, int held, int up) {return 0;} // Inherited classes override this
};

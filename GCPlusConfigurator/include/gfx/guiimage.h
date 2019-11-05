#pragma once

#include "gui.h"
#include "draw.h"

class GuiImage : public GuiElement {
    private:
        Texture tex;
        int width;
        int height;
    public:
        GuiImage();
        GuiImage(const char* filename);
        GuiImage(const GuiImage& img);
        ~GuiImage();
        //GuiImage(const char* filename, int _width, int _height);
        void loadImage(const char* filename);
        void setSize(int _width, int _height);
        void draw();
        Rect getRect() {return Rect(0, 0, width, height);};

        GuiImage& operator = (const GuiImage& img);
};
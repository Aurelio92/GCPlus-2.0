#pragma once

#include <gccore.h>
#include "gui.h"
#include "draw.h"
#include "plot.h"

namespace Gfx
{
	void init();
	void startDrawing();
    void fillScreen();
	void endDrawing();
	void concatViewMatrix(Mtx mi, Mtx mo);
    void restoreViewMatrix();
    void translate(f32 x, f32 y);
    void pushMatrix();
    void popMatrix();
    void getCurMatrix(Mtx out);
    Rect getCurScissorBox();
    void pushScissorBox(u32 w, u32 h);
    void popScissorBox();
}
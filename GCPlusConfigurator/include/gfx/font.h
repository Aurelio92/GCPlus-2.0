#pragma once

#include <gccore.h>
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "draw.h"

class Font {
    public:
        struct My_GlyphSlot {
                Texture tex;
                u16 width, height;
                FT_Glyph_Metrics metrics;
                FT_Vector advance;
                int bitmap_top, bitmap_left;

        };

    private:
        FT_Face face;
        u8* faceMemory;
        u16 size;
        std::map<wchar_t, My_GlyphSlot *> loadedGlyphs;
        u32 color;

        My_GlyphSlot* loadChar(wchar_t charCode);

    public:
        static FT_Library library;
        Font();
        Font(const char* filename, u16 size);
        Font(const u8* fontData, const u32 fontSize, u16 size);
        ~Font();

        u16 getSize() {
            return size;
        }

        void setColor(u32 _color) {
            color = _color;
        }

        u32 getColor() {
            return color;
        }

        int drawChar(int x, int y, wchar_t charCode);
        int getCharWidth(wchar_t charCode);
        int printf(int x, int y, const wchar_t * format, ...);
        int printf(int x, int y, const char * format, ...);
        int getTextWidth(const wchar_t * format, ...);
        int getTextWidth(const char * format, ...);

        /*My_GlyphSlot* loadChar(FT_ULong charCode);
        u32 getString16Width(const std::u16string& str);
        int drawChar(s16 x0, s16 y0, FT_ULong charCode);
        void drawCharBounded(s16* x0, s16* y0, Rect& rect, FT_ULong charCode);
        void drawTexture(s16 x0, s16 y0);
        int printf16(Rect rect, const std::u16string& format, ...);*/
        //int printf16(s16 x0, s16 y0, std::u16string s);
};
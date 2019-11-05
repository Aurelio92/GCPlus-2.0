#include "font.h"

FT_Library Font::library = NULL;

Font::Font() {
    this->face = NULL;
    faceMemory = NULL;

    //TODO: Error handler
    FT_Error error;

    if (Font::library == NULL) { //Initialize freetype library only if it isn't already
        error = FT_Init_FreeType(&(Font::library));
    }

    color = RGBA8(0xFF, 0xFF, 0xFF, 0xFF);
}

Font::Font(const char* filename, u16 size) {
    //TODO: Error handler
    FT_Error error;
    this->face = NULL;
    faceMemory = NULL;

    if (Font::library == NULL) { //Initialize freetype library only if it isn't already
        error = FT_Init_FreeType(&(Font::library));
    }

    //Load font
    //error = FT_New_Face(Font::library, filename, 0, &face);
    FILE* fp = fopen(filename, "rb");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        u32 fileSize = ftell(fp);
        rewind(fp);
        faceMemory = (u8*)malloc(fileSize);
        fread(faceMemory, 1, fileSize, fp);
        fclose(fp);
        error = FT_New_Memory_Face(Font::library, faceMemory, fileSize, 0, &face);

        //Set font size
        error = FT_Set_Pixel_Sizes(this->face, 0, size);
        this->size = size;

        color = RGBA8(0xFF, 0xFF, 0xFF, 0xFF);
    }
}

Font::Font(const u8* fontData, const u32 fontSize, u16 size) {
    //TODO: Error handler
    FT_Error error;
    this->face = NULL;
    faceMemory = NULL;

    if (Font::library == NULL) { //Initialize freetype library only if it isn't already
        error = FT_Init_FreeType(&(Font::library));
    }

    //Load font
    //error = FT_New_Face(Font::library, filename, 0, &face);
    faceMemory = (u8*)malloc(fontSize);
    memcpy(faceMemory, fontData, fontSize);
    error = FT_New_Memory_Face(Font::library, faceMemory, fontSize, 0, &face);

    //Set font size
    error = FT_Set_Pixel_Sizes(this->face, 0, size);
    this->size = size;

    color = RGBA8(0xFF, 0xFF, 0xFF, 0xFF);
}

Font::~Font() {
    //Deallocate memory for face
    if (face != NULL) FT_Done_Face(face);
    face = NULL;
    if (faceMemory != NULL) free(faceMemory);
    faceMemory = NULL;

    //Delete all the loaded glyphs
    for (auto it : loadedGlyphs) {
        //Second contains the actual glyph
        delete it.second;
    }
}

Font::My_GlyphSlot* Font::loadChar(wchar_t charCode) {
    //TODO: Error handler
    FT_Error error;

    //Check if there is no library or face initialized
    if (Font::library == NULL || face == NULL) return NULL;

    My_GlyphSlot* glyph;

    //Check if the needed glyph is already loaded
    auto searchGlyph = loadedGlyphs.find(charCode);
    if (searchGlyph != loadedGlyphs.end()) { //A glyph has been found!
        glyph = searchGlyph->second; //searchGlyph->second contains the actual glyph, while searchGlyph->first contains the key (charCode in our case)
    }
    else { //No glyph corresponding to our charCode has already been loaded from the font => Load it!
        //Load character glyph
        error = FT_Load_Char(face, charCode, FT_LOAD_RENDER); //TODO: Error check!
        FT_GlyphSlot tempGlyph = face->glyph;
        FT_Bitmap* glyphBitmap = &(tempGlyph->bitmap);

        //Copy the loaded glyph in our loadedGlyphs map
        glyph = new My_GlyphSlot();
        glyph->metrics = tempGlyph->metrics;
        glyph->advance = tempGlyph->advance;
        glyph->width = glyphBitmap->width;
        glyph->height = glyphBitmap->rows;
        glyph->bitmap_top = tempGlyph->bitmap_top;
        glyph->bitmap_left = tempGlyph->bitmap_left;

        glyph->tex = createTextureA8(glyphBitmap->buffer, glyph->width, glyph->height);
        loadedGlyphs[charCode] = glyph;
    }

    return glyph;
}

int Font::drawChar(int x, int y, wchar_t charCode)
{
    //Check if there is no library or face initialized
    if (Font::library == NULL || face == NULL) return -1;

    //Load char from font file or font texture
    My_GlyphSlot* glyph = loadChar(charCode);

    drawTextureColor(x + glyph->bitmap_left, y + this->size - glyph->bitmap_top, color, glyph->tex);
    return glyph->advance.x >> 6;
}

int Font::getCharWidth(wchar_t charCode)
{
    //Check if there is no library or face initialized
    if (Font::library == NULL || face == NULL) return -1;

    //Load char from font file or font texture
    My_GlyphSlot* glyph = loadChar(charCode);

    return glyph->advance.x >> 6;
}

int Font::printf(int x, int y, const wchar_t * format, ...) {
    wchar_t buffer[256];
    va_list args;
    size_t len;
    size_t i;

    va_start(args, format);
    vswprintf(buffer, 256, format, args);
    len = wcslen(buffer);
    for (i = 0; i < len; i++) {
        x += drawChar(x, y, buffer[i]);
    }
    va_end(args);

    return x;
}

int Font::printf(int x, int y, const char * format, ...) {
    char buffer[256];
    va_list args;
    size_t len;
    size_t i;

    va_start(args, format);
    vsnprintf(buffer, 256, format, args);
    len = strlen(buffer);
    for (i = 0; i < len; i++) {
        x += drawChar(x, y, buffer[i]);
    }
    va_end(args);

    return x;
}

int Font::getTextWidth(const wchar_t * format, ...) {
    wchar_t buffer[256];
    va_list args;
    size_t len;
    size_t i;
    int x = 0;

    va_start(args, format);
    vswprintf(buffer, 256, format, args);
    len = wcslen(buffer);
    for (i = 0; i < len; i++) {
        x += getCharWidth(buffer[i]);
    }
    va_end(args);

    return x;
}

int Font::getTextWidth(const char * format, ...) {
    char buffer[256];
    va_list args;
    size_t len;
    size_t i;
    int x = 0;

    va_start(args, format);
    vsnprintf(buffer, 256, format, args);
    len = strlen(buffer);
    for (i = 0; i < len; i++) {
        x += getCharWidth(buffer[i]);
    }
    va_end(args);

    return x;
}
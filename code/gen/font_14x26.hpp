typedef struct glyph
{
    int x, y;
    int width, height;
    int origin_x, origin_y;
} glyph;

typedef struct font
{
    int width, height;
} font;

glyph get_glyph(char win1251_encoded_char)
{
    int row = (win1251_encoded_char - 32) / 16;
    int col = (win1251_encoded_char - 32) % 16;

    glyph result;
    result.x = col * 14;
    result.y = row * 26;
    result.width = 14;
    result.height = 26;
    result.origin_x = 7;
    result.origin_y = 22;

    return result;
}

static font font_14x26 = { 14*16, 26*14 };

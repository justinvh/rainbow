#ifndef RAINBOW_DISPLAY_FONT_HPP
#define RAINBOW_DISPLAY_FONT_HPP

#include <vector>
#include <map>

#include <rainbow/gl.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_SIZES_H

namespace rb {

typedef float Glyph[8];
typedef std::vector<Glyph> Glyph_vector;
typedef std::map<uint32_t, Glyph> Char_glyph_map;
typedef std::map<uint32_t, Char_glyph_map> Size_glyph_map;

struct FT {
    FT_Library ft_library;
    GLuint texture_id;
    GLint texture_pen[2];
    GLint texture_row;
    GLint texture_size[2];
    struct Context {
        GLuint maximum_texture_size;
    };
};

class Font {
public:
    // Most common operatons
    Font() = default;
    Font(const std::string& filename, uint32_t index);
    bool load(const std::string& filename, uint32_t index);
    float size(uint32_t font_size);
    const Glyph* get(uint32_t font_size, char character);
    Char_glyph_map& operator[](uint32_t font_size);
    const Glyph* operator[](char character);
    Glyph_vector operator[](const std::string& characters);
public:
    // More advanced methods
    float advance(uint32_t lg, uint32_t rg);
    float height();
    float ascender();
    float descender();
public:
    Size_glyph_map glyphs;
    FT* ft;
};

}

#endif // RAINBOW_DISPLAY_FONT_HPP

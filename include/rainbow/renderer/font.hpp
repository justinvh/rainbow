#pragma once
#ifndef RAINBOW_DISPLAY_FONT_HPP
#define RAINBOW_DISPLAY_FONT_HPP

#include <vector>
#include <map>

#include <rainbow/gl.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_SIZES_H

namespace rb {

struct Glyph {
    int id;
    GLint texture;
    FT_Glyph_Metrics metrics;
    FT_Vector advance;
    GLfloat vertices[20];
    GLint elements[6];
};

typedef std::vector<const Glyph*> Glyph_vector;
typedef std::map<uint32_t, Glyph> Char_glyph_map;
typedef std::map<uint32_t, Char_glyph_map> Size_glyph_map;

struct FT {
    FT() = default;
    ~FT()
    {
        if (library) {
            FT_Done_FreeType(library);
            glDeleteTextures(1, &texture.id);
        }
    }

    FT_Library library;

    struct Pen {
        GLint x;
        GLint y;
    } pen;

    struct Texture {
        GLuint id;
        GLint rows;
        GLint width;
        GLint height;
    } texture;

    struct Context {
        GLuint maximum_texture_size;
    } context;
};

class Font {
public:
    // Most common operatons
    ~Font();
    Font() = default;
    Font(const std::string& filename, uint32_t index);
    bool valid();
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
    FT_Face ft_face;
    float ft_size;
    bool valid_font;
};

}

#endif // RAINBOW_DISPLAY_FONT_HPP

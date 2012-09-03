#include <iostream>
#include <memory>
#include <map>
#include <rainbow/gl.h>
#include <rainbow/renderer/font.hpp>

using namespace rb;
using namespace std;

namespace {
    typedef map<string, unique_ptr<FT>> Library_mapping;
    Library_mapping font_library;
}

bool expand_texture(FT* ft)
{
    glGetError();
    if (ft->texture.width == 0 || ft->texture.height == 0) {
        int width = ft->context.maximum_texture_size;
        int height = 128;

        ft->texture.width = width;
        ft->texture.height = height;

        size_t allocation = width * height;
        auto data = unique_ptr<GLubyte>(new GLubyte[allocation]);
        memset(data.get(), 0, sizeof(GLubyte) * allocation);

        glTexImage2D(GL_TEXTURE, 0, GL_R8, 
                width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data.get());

        if (glGetError()) {
            cerr << "Could not format texture" << endl;
            return false;
        }

        glGetTexLevelParameteriv(GL_TEXTURE, 0, 
                                 GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(GL_TEXTURE, 0, 
                                 GL_TEXTURE_HEIGHT, &height);

        if (width == 0 || height == 0) {
            cerr << "Failed to create texture with dimensions: "
                 << width << "x" << height << endl;
            return false;
        }

        ft->texture.width = width;
        ft->texture.height = height;
        ft->pen.x = 1;
        ft->pen.y = 1;
        ft->texture.rows = 0;
    } else {
        size_t alloc = ft->texture.width * ft->texture.height;
        auto buffer = unique_ptr<GLubyte>(new GLubyte[alloc]);

        glGetTexImage(GL_TEXTURE, 0, GL_RED, 
            GL_UNSIGNED_BYTE, buffer.get());

        int original_texture_height = ft->texture.height;
        ft->texture.height *= 2;
        if (ft->texture.height > ft->context.maximum_texture_size)
            ft->texture.height = ft->context.maximum_texture_size;

        alloc = ft->texture.width * ft->texture.height;
        auto data = unique_ptr<GLubyte>(new GLubyte[alloc]);
        memset(data.get(), 0, alloc);

        glTexImage2D(GL_TEXTURE, 0, GL_R8, ft->texture.width, 
            ft->texture.height, 0, GL_RED, GL_UNSIGNED_BYTE, data.get());

		glTexSubImage2D(GL_TEXTURE, 0, 0, 0, 
            ft->texture.width, original_texture_height, GL_RED,
            GL_UNSIGNED_BYTE, buffer.get());

        glGetTexLevelParameteriv(GL_TEXTURE, 0, 
            GL_TEXTURE_WIDTH, &ft->texture.width);

		glGetTexLevelParameteriv(GL_TEXTURE, 0, 
            GL_TEXTURE_HEIGHT, &ft->texture.height);
    }

    return true;
}

FT* ft_load_library(const string& filename)
{
    glGetError();
    auto library = font_library.find(filename);
    if (library != font_library.end())
        return library->second.get();

    // Initialize the font library
    unique_ptr<FT> ft_ptr(new FT);
    FT& ft = *ft_ptr.get();
    FT_Error err = FT_Init_FreeType(&ft.library);
    if (err) {
        return nullptr;
    }

    // Initialize the GL textures
    GLint texture_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texture_size);
    ft.context.maximum_texture_size = texture_size;
    if (ft.context.maximum_texture_size == 0) {
        cerr << "Failed to set maximum texture size." << endl;
        return nullptr;
    }

    // Generate a new texture
    glGenTextures(1, &ft.texture.id);
    int glerr = glGetError();
    if (glerr) {
        cerr << "Failed to create a texture: " << glerr << endl;
        return nullptr;
    }

    font_library[filename] = std::move(ft_ptr);
    return font_library[filename].get();
}

Font::Font(const string& filename, uint32_t index)
{
    valid_font = load(filename, index);
}

Font::~Font()
{
    FT_Done_Face(ft_face);
}

bool Font::load(const string& filename, uint32_t index)
{
    ft = ft_load_library(filename);
    valid_font = ft != nullptr;
    if (!valid_font)
        return false;

    FT_Error err = FT_New_Face(ft->library, filename.c_str(), 
                                  index, &ft_face);
    if (err) {
        cerr << "Failed to load ft_face for " << filename << " with error "
             << err << endl;
        valid_font = false;
        return false;
    }

    return valid_font;
}

float Font::size(uint32_t font_size)
{
    ft_size = font_size;
    FT_Set_Char_Size(ft_face, font_size * 64.0f, font_size * 64.0f, 0, 0);
    return ft_size;
}

const Glyph* Font::get(uint32_t font_size, char character)
{
    auto size_glyphs = glyphs.find(font_size);

    if (size_glyphs != glyphs.end()) {
        auto glyph = size_glyphs->second.find(character);
        if (glyph != size_glyphs->second.end())
            return &glyph->second;
    }

    ft_size = font_size;
    FT_Set_Char_Size(ft_face, font_size * 64.0f, font_size * 64.0f, 0, 0);

    int32_t ft_mask = (FT_LOAD_DEFAULT | FT_LOAD_IGNORE_TRANSFORM 
                       | FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT);
    FT_Error err = FT_Load_Char(ft_face, character, ft_mask);
    if (err) {
        cerr << "Failed to load character: " << character << endl;
        return nullptr;
    }

    GLint last_texture_id;
    glGetIntegerv(GL_TEXTURE_BINDING_RECTANGLE, &last_texture_id);

    if (ft->texture.width == 0 || ft->texture.height == 0)
        if (!expand_texture(ft))
            return nullptr;

    FT_Bitmap* bitmap = &ft_face->glyph->bitmap;
    if ((ft->pen.x + bitmap->width + 1) > ft->texture.width) {
        ft->pen.y += ft->texture.rows + 1;
        ft->pen.x = 1;
        ft->texture.rows = 0;

        if ((ft->pen.y + bitmap->rows + 1) > ft->texture.height) {
            expand_texture(ft);
        }
    }

    size_t alloc = bitmap->width * bitmap->rows;
    auto data = unique_ptr<GLubyte>(new GLubyte[alloc]);

    for (int y = 0, i = 0; y < bitmap->rows; y++) {
        for (int x = 0; x < bitmap->width; x++) {
            int col = x + (bitmap->rows - 1 - y) * bitmap->width;
            data.get()[col] = bitmap->buffer[i++];
        }
    }

    GLint uplast;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &uplast);

    if (uplast != 1) 
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexSubImage2D(GL_TEXTURE, 0, ft->pen.x, ft->pen.y,
        bitmap->width, bitmap->rows, GL_RED, GL_UNSIGNED_BYTE, data.get());

    if (uplast != 1)
        glPixelStorei(GL_UNPACK_ALIGNMENT, uplast);

    if ((GLuint)last_texture_id != ft->texture.id)
        glBindTexture(GL_TEXTURE, last_texture_id);

    Glyph& glyph = glyphs[ft_size][character];
    glyph.id = FT_Get_Char_Index(ft_face, (const FT_ULong)character);
    glyph.texture = ft->texture.id;
    glyph.metrics = ft_face->glyph->metrics;
    glyph.advance = ft_face->glyph->advance;

    GLfloat l = ft_face->glyph->bitmap_left - 0.5f;
    GLfloat b = -bitmap->rows + ft_face->glyph->bitmap_top - 0.5f;
    GLfloat r = bitmap->width + ft_face->glyph->bitmap_left + 0.5f;
    GLfloat t = -bitmap->rows + ft_face->glyph->bitmap_top + bitmap->rows + 0.5f;
    GLfloat ul = ft->pen.x - 0.5f;
    GLfloat ub = ft->pen.y - 0.5f;
    GLfloat ur = ft->pen.x + bitmap->width + 0.5f;
    GLfloat ut = ft->pen.y + bitmap->rows + 0.5f;

    GLfloat vertices[20] = {
    //  x  y  z         u   v
        l, b, 0.0f,     ul, ub,     // bl
        l, t, 0.0f,     ul, ut,     // tl
        r, t, 0.0f,     ur, ut,     // tr
        r, b, 0.0f,     ur, ub,     // br
    };

    GLint elements[6] = {
        0, 1, 3,
        3, 2, 1
    };

    memcpy(&glyph.vertices, vertices, sizeof(glyph.vertices));
    memcpy(&glyph.elements, elements, sizeof(glyph.elements));

    ft->pen.x += bitmap->width + 1;
    
    if (bitmap->rows > ft->texture.rows)
        ft->texture.rows = bitmap->rows;

    return &glyph;
}

Char_glyph_map& Font::operator[](uint32_t font_size)
{
    return glyphs[font_size];
}

const Glyph* Font::operator[](char character)
{
    return get(ft_size, character);
}

Glyph_vector Font::operator[](const string& characters)
{
    Glyph_vector glyphs;
    for (char character : characters) {
        glyphs.push_back(get(ft_size, character));
    }

    return glyphs;
}

float Font::advance(uint32_t lg, uint32_t rg)
{
    return 1.0f;
}

float Font::height()
{
    return 1.0f;
}

float Font::ascender()
{
    return 1.0f;
}

float Font::descender()
{
    return 1.0f;
}

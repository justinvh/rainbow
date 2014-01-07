#include <SDL_ttf.h>

#include <iostream>
#include <cassert>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <cmath>

#include <rainbow/gl.h>
#include <rainbow/state.hpp>
#include <rainbow/renderer.hpp>
#include <rainbow/display/gui.hpp>
#include <rainbow/file.hpp>

using namespace std;
using namespace rb;

void Gui::draw(int x, int y, const std::string& message,
               const std::string& shader_name)
{
    /*
    SDL_Color black = {0xFF, 0xFF, 0xFF, 0};
    TTF_Font* font = TTF_OpenFont("game/fonts/WellrockSlab.ttf", 32);
    SDL_Surface* font_surface = TTF_RenderText_Solid(font, message.c_str(),
                                                     black);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    SDL_Surface* tmp = SDL_CreateRGBSurface(0, font_surface->w,
        font_surface->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_BlitSurface(font_surface, nullptr, tmp, nullptr);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp->w, tmp->h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, tmp->pixels);

    float l = x / 640.0f - 1.0;
    float t = -y / 480.0f + 1.0;
    float r = l + font_surface->w / 640.0f;
    float b = t - font_surface->h / 480.0f;

    std::cout << "Drawing " << x << " " << y << ": " << message << " at "
        << "l: " << l << ", t: " << t << ", r: " << r << ", b: " << b
        << "\n";

    float vertices[] = {
        //  Position      Color             Texcoords
        l, t, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
        r, t, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
        r, b, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
        l, b, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
    };

    int elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    Shader* shader = state->renderer->get_or_create_shader(shader_name);
    state->renderer->add_static_vertices(vertices, sizeof(vertices),
        elements, sizeof(elements),
        [shader, texture]() {
            shader->use();
            shader->attrib("position").vec2(7, 0);
            shader->attrib("color").vec3(7, 2);
            shader->attrib("texcoord").vec2(7, 5);
            Uniform tex = shader->uniform("tex");
            glBindTexture(GL_TEXTURE_2D, texture);
            tex.int1(0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        });
    */
}

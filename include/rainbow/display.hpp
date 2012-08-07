#ifndef RAINBOW_DISPLAY_HPP
#define RAINBOW_DISPLAY_HPP

#include <SDL.h>
#include <OpenGL/OpenGL.h>
#include <SDL_opengl.h>

namespace rb {

class Display {
public:
    Display(const char* display_name);
    bool resolution(int width, int height);
    void run();
public:
    const char* display_name;
    char driver_name[64];
    SDL_Surface* screen;
    CGLContextObj context;
};

}

#endif // RAINBOW_DISPLAY_HPP

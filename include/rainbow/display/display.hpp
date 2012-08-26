#ifndef RAINBOW_DISPLAY_DISPLAY_HPP
#define RAINBOW_DISPLAY_DISPLAY_HPP

#include <rainbow/gl.h>

namespace rb {

class Renderer;

class Display {
public:
    Display(const char* display_name);
    ~Display();
    bool resolution(int width, int height);
    void run();
    void clear();
    void end_frame();
public:
    const char* display_name;
    char driver_name[64];
    SDL_Window* screen;
    SDL_GLContext context;
    Renderer* renderer;
};

}

#endif // RAINBOW_DISPLAY_DISPLAY_HPP

#pragma once
#ifndef RAINBOW_DISPLAY_DISPLAY_HPP
#define RAINBOW_DISPLAY_DISPLAY_HPP

#include <rainbow/gl.h>
#include <chrono>

/* Bug in current libc++11 that prevents typedef'ng
typedef std::chrono::time_point<std::chrono::high_resolution_clock> Time_point;
typedef std::chrono::high_resolution_clock Timer;
typedef std::chrono::duration_cast<std::chrono::seconds> Timer_diff;
*/

#define Time_point std::chrono::time_point<std::chrono::high_resolution_clock>
#define Timer std::chrono::high_resolution_clock
#define Timer_diff std::chrono::duration_cast<std::chrono::milliseconds>

namespace rb {

struct Timing {
    Time_point timer;
    float fps;
};

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
    Timing timing;
    bool grab_mouse;
    int screen_center_x;
    int screen_center_y;
};

}

#endif // RAINBOW_DISPLAY_DISPLAY_HPP

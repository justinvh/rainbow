#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <chrono>
#include <rainbow/gl.h>
#include <rainbow/renderer.hpp>
#include <rainbow/display.hpp>
#include <rainbow/math/random.hpp>

using namespace rb;
using namespace std;

Display::~Display()
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(screen);
    delete renderer;
}

Display::Display(const char* display_name)
        : display_name(display_name), grab_mouse(false)
{
    timing.timer = Timer::now();
    screen = nullptr;
    random_seed();

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        throw SDL_GetError();

    TTF_Init();
    renderer = new Renderer(this);
}

bool Display::resolution(int width, int height)
{
    if (screen != nullptr) {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(screen);
    }

    screen = SDL_CreateWindow("rainbow", SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED, 640, 480, 
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    screen_center_x = width / 2;
    screen_center_y = height / 2;

    if (!screen)
        return false;
    
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    context = SDL_GL_CreateContext(screen);

    // vsync
    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) 
        cerr << glewGetErrorString(glew_status) << endl;

    renderer->init();
    return true;
}

void Display::run()
{
    Time_point dt = Timer::now();
    timing.fps = 1.0f / Timer_diff(dt - timing.timer).count() * 1000.0f;
    timing.timer = dt;
    if (grab_mouse) {
        SDL_ShowCursor(0);
    } else {
        SDL_ShowCursor(1);
    }
    renderer->run_frame();
}

void Display::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::end_frame()
{
    SDL_GL_SwapWindow(screen);
}

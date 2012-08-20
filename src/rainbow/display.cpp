#include <iostream>
#include <rainbow/renderer.hpp>
#include <rainbow/display.hpp>

using namespace rb;
using namespace std;

Display::~Display()
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(screen);
    delete renderer;
}

Display::Display(const char* display_name)
        : display_name(display_name)
{
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        throw SDL_GetError();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    screen = SDL_CreateWindow("rainbow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!screen)
        throw "Failed to set video mode";

    context = SDL_GL_CreateContext(screen);

    // vsync
    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) 
        cerr << glewGetErrorString(glew_status) << endl;
    renderer = new Renderer(this);
}

bool Display::resolution(int width, int height)
{
    screen = SDL_CreateWindow("rainbow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!screen)
        throw "Failed to set video mode";
    context = SDL_GL_CreateContext(screen);
    return true;
}

void Display::run()
{
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

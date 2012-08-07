#include <iostream>
#include <rainbow/display.hpp>

using namespace rb;
using namespace std;

Display::Display(const char* display_name)
        : display_name(display_name)
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1) 
        throw SDL_GetError();

    SDL_VideoDriverName(driver_name, sizeof(driver_name) - 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    screen = SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
    if (!screen)
        throw "Failed to set video mode";

    context = CGLGetCurrentContext();

    SDL_SetGamma(1.0f, 1.0f, 1.0f);
    glClearDepth(1.0f);
    glCullFace(GL_FRONT_AND_BACK);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glDepthFunc(GL_LEQUAL);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthMask(GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    glDisable(GL_LIGHTING);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    cout << "Driver     : " << driver_name << "\n"
         << "Renderer   : " << glGetString(GL_RENDERER) << "\n"
         << "Vendor     : " << glGetString(GL_VENDOR) << "\n"
         << "Version    : " << glGetString(GL_VERSION) << "\n"
         << "Extensions : " << glGetString(GL_EXTENSIONS) << "\n";
}

bool Display::resolution(int width, int height)
{
    screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL);
    return true;
}

void Display::run()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapBuffers();
}

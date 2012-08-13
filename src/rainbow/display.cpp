#include <iostream>
#include <rainbow/renderer.hpp>
#include <rainbow/display.hpp>

using namespace rb;
using namespace std;

Display::~Display()
{
    delete renderer;
}

Display::Display(const char* display_name)
        : display_name(display_name)
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1) 
        throw SDL_GetError();

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    screen = SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
    if (!screen)
        throw "Failed to set video mode";

    context = CGLGetCurrentContext();
    renderer = new Renderer(this);
    SDL_SetGamma(1.0f, 1.0f, 1.0f);
}

bool Display::resolution(int width, int height)
{
    screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL);
    return true;
}

void Display::run()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    test();
    SDL_GL_SwapBuffers();
}

void Display::run_forever()
{
    while (true) {
        run();
    }
}

void Display::test()
{
    Surface_triangles tris;
    tris.verts = new Surface_vertex[4];
    tris.indexes = new int[4];
    tris.num_indexes = 4;
    tris.num_verts = 4;

    tris.verts[0] = (Surface_vertex){{0, 0}, (float [3]) {0, 0, 0}};

    tris.indexes[0] = 0;
    tris.verts[1].xyz = (float [3]) {-1, 0, 0};
    tris.indexes[1] = 1;
    tris.verts[2].xyz = (float [3]) {1, 1, 0};
    tris.indexes[2] = 2;
    tris.verts[3].xyz = (float [3]){0, -1, 0};
    tris.indexes[3] = 3;

    renderer->draw_elements_immediate(&tris);

    delete[] tris.verts;
    delete[] tris.indexes;
}

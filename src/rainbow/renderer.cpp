#include <iostream>
#include <sstream>

#include <SDL.h>
#include <OpenGL/OpenGL.h>
#include <SDL_opengl.h>

#include <rainbow/display.hpp>
#include <rainbow/renderer.hpp>

using namespace rb;
using namespace std;

Renderer::Renderer(Display* display)
{
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

    // Store our GL context information
    char driver_name[64];
    SDL_VideoDriverName(driver_name, sizeof(driver_name) - 1);
    info.driver_name = driver_name;
    info.renderer = (const char*)glGetString(GL_RENDERER);
    info.vendor = (const char*)glGetString(GL_VENDOR);
    info.version = (const char*)glGetString(GL_VERSION);
    
    cout << "Driver     : " << driver_name << "\n"
         << "Renderer   : " << info.renderer << "\n"
         << "Vendor     : " << info.vendor << "\n"
         << "Version    : " << info.version << "\n";

    // Create a mappable extensions array for quickly querying
    std::stringstream iss((const char*)glGetString(GL_EXTENSIONS));
    std::string extension;
    cout << "Extensions : ";
    for (int i = 1; iss >> extension; i++) {
        if (i % 3 == 0)  
            cout << "\n             ";
        cout  << extension << " ";
        info.extensions[extension] = true;
    }
    cout << "\n";
}

/**
 * draw_elements_immediate
 *
 * Draws with an immediate mode command. This is basically for testing
 * purposes as outside the realm of the vertex cache, this will operate
 * horribly.
 */
void Renderer::draw_elements_immediate(const Surface_triangles* tri)
{
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < tri->num_indexes; i++) {
        glTexCoord2fv(tri->verts[tri->indexes[i]].st);
        glVertex3fv(tri->verts[tri->indexes[i]].xyz);
    }
    glEnd();
}

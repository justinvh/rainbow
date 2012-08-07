#include <SDL.h>
#include <OpenGL/OpenGL.h>
#include <SDL_opengl.h>

#include <rainbow/renderer.hpp>

using namespace rb;

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

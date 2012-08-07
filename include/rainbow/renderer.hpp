#ifndef RAINBOW_RENDERER_HPP
#define RAINBOW_RENDERER_HPP

namespace rb {

struct Surface_vertex {
    float st[2];
    float xyz[3];
};

struct Surface_triangles {
    Surface_vertex* verts;
    int* indexes;
    int num_indexes;
    int num_verts;
};

class Renderer {
public:
    void draw_elements_immediate(const Surface_triangles* triangles);
};

}

#endif // RAINBOW_RENDERER_HPP

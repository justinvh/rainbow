#ifndef RAINBOW_RENDERER_HPP
#define RAINBOW_RENDERER_HPP

#include <string>
#include <map>

namespace rb {

class Display;

struct Surface_vertex {
    float st[2];
    float* xyz;
};

struct Surface_triangles {
    Surface_vertex* verts;
    int* indexes;
    int num_indexes;
    int num_verts;
};

typedef std::map<std::string, bool> GL_extensions;
struct GL_info {
    std::string driver_name;
    std::string renderer;
    std::string vendor; 
    std::string version;
    GL_extensions extensions;
};

class Renderer {
public:
    Renderer() = default;
    Renderer(Display* display);
    void draw_elements_immediate(const Surface_triangles* triangles);
public:
    Display* display;
    GL_info info;
};

}

#endif // RAINBOW_RENDERER_HPP

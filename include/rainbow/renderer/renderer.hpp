#ifndef RAINBOW_RENDERER_RENDERER_HPP
#define RAINBOW_RENDERER_RENDERER_HPP

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <exception>

#include <rainbow/gl.h>
#include <rainbow/renderer/shader.hpp>

namespace rb {

class Display;
class Renderer;

typedef std::map<std::string, bool> GL_extensions;
struct GL_info {
    std::string driver_name;
    std::string renderer;
    std::string vendor; 
    std::string version;
    std::string glsl;
    GL_extensions extensions;
};

class Renderer_tests {
public:
    void color_square();

public:
    Renderer* renderer;
};

struct Static_entry {
    size_t begin, end, count, offset;
    std::function<void(void)> setup;
};

typedef std::map<int, std::unique_ptr<Shader>> Shader_map;
class Renderer {
public:
    Renderer() = default;
    Renderer(Display* display);
    void init();
    void run_frame();
    GLuint add_static_vertices(const float* vertices, uint32_t vlength,
                               const int* elements, uint32_t elength,
                               std::function<void(void)> setup = [](){});
    GLuint add_dynamic_vertices(float* vertices, uint32_t length);
    GLuint add_stream_vertices(float* vertices, uint32_t length);
    Shader* get_or_create_shader(const std::string& name);
    int add_shader(const std::string& name,
                   const std::string& vertex, 
                   const std::string& fragment,
                   bool raise_exception);
public:
    Display* display;
    Shader_map shaders;
    GL_info info;

public:
    std::vector<Static_entry> static_draws;
    uint32_t total_elements;
    uint32_t total_vertices;
    Renderer_tests tests;

public:
    GLuint vao_static;
    GLuint vbo_static;
    GLuint ebo_static;
    float* vertices_static;
    size_t vertices_static_size;
    int* elements_static;
    size_t elements_static_size;
};

}

#endif // RAINBOW_RENDERER_RENDERER_HPP

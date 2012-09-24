#include <iostream>
#include <random>
#include <sstream>
#include <cstdint>

#include <rainbow/display.hpp>
#include <rainbow/renderer.hpp>

using namespace rb;
using namespace std;

Renderer::Renderer(Display* display)
{
    total_elements = 0;
    total_vertices = 0;
    tests.renderer = this;
}

void Renderer::init()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glDepthMask(GL_TRUE);
    glEnable(GL_TEXTURE_2D);

    glGenVertexArrays(1, &vao_static);
    glGenBuffers(1, &vbo_static);
    glGenBuffers(1, &ebo_static);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    vertices_static = (float*)(malloc(16 * sizeof(float)));
    vertices_static_size = 0;
    elements_static = (int*)(malloc(16 * sizeof(int)));
    elements_static_size = 0;

    // Store our GL context information
    info.driver_name = SDL_GetCurrentVideoDriver();
    info.renderer = (const char*)glGetString(GL_RENDERER);
    info.vendor = (const char*)glGetString(GL_VENDOR);
    info.version = (const char*)glGetString(GL_VERSION);
    info.glsl = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    cout << "Driver     : " << info.driver_name << "\n"
         << "Renderer   : " << info.renderer << "\n"
         << "Vendor     : " << info.vendor << "\n"
         << "Version    : " << info.version << "\n"
         << "GLSL       : " << info.glsl << "\n";

    // Create a mappable extensions array for quickly querying
    const GLubyte* extensions = glGetString(GL_EXTENSIONS);
    cout << "Extensions : ";
    if (extensions == nullptr) {
        cerr << "None" << endl;
    } else {
        std::stringstream iss((const char*)extensions);
        std::string extension;
        for (int i = 1; iss >> extension; i++) {
            if (i % 3 == 0)  
                cout << "\n             ";
            cout  << extension << " ";
            info.extensions[extension] = true;
        }
        cout << "\n";
    }
}

void Renderer::camera_frame()
{
    if (!camera)
        return;

    camera->shader->use();
}

void Renderer::run_frame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_static);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_static);

        // Final pass
    camera_frame();
    for (Static_entry entry : static_draws) {
        glDrawRangeElements(GL_TRIANGLES, 
                            entry.begin,
                            entry.end,
                            entry.count,
                            GL_UNSIGNED_INT, 
                            (const GLuint*)0 + entry.offset);
    }

    // Static pass
    for (Static_entry entry : static_draws) {
        entry.setup();
        glDrawRangeElements(GL_TRIANGLES, 
                            entry.begin,
                            entry.end,
                            entry.count,
                            GL_UNSIGNED_INT, 
                            (const GLuint*)0 + entry.offset);
    }
}

GLuint Renderer::add_static_vertices(const float* vertices, 
                                     uint32_t vlength,
                                     const int* elements,
                                     uint32_t elength,
                                     std::function<void(void)> setup)
{
    
    glBindVertexArray(vao_static);

    // Reallocate the vertices
    int realloc_vsize = vertices_static_size + vlength;
    float* t = (float*)malloc(realloc_vsize);
    memcpy(t, vertices_static, vertices_static_size);
    memcpy(t + vertices_static_size / sizeof(int), vertices, vlength);
    free(vertices_static);
    vertices_static = t;

    // Bind the new buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_static);
    glBufferData(GL_ARRAY_BUFFER, realloc_vsize, 
        vertices_static, GL_STATIC_DRAW);
    
    // Reallocate the elements
    int realloc_esize = elements_static_size + elength;
    int* s = (int*)malloc(realloc_esize);
    memcpy(s, elements_static, elements_static_size);
    memcpy(s + elements_static_size / sizeof(int), elements, elength);
    free(elements_static);
    elements_static = s;

    // adjust the elements data
    for (int i = 0; i < elength / sizeof(int); i++)  {
        size_t offset = elements_static_size / sizeof(int);
        elements_static[i + offset] += (vertices_static_size / sizeof(int) / 7);
    }

    // adjust the elements data
    for (int i = 0; i < realloc_esize / sizeof(int); i++) 
        std::cout << elements_static[i] << " ";

    // Update the elements array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_static);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, realloc_esize, 
        elements_static, GL_STATIC_DRAW);

    // Create a new static entry draw routine
    size_t element_offset = elements_static_size / sizeof(GLuint);
    size_t element_count = (realloc_esize-elements_static_size)/sizeof(GLuint);
    Static_entry entry = {elements_static_size,  // begin
                          realloc_esize,         // end
                          element_count,         // count
                          element_offset,        // offset
                          setup};                // setup

    // Add to draw list
    static_draws.push_back(entry);

    // Update the new static allocator sizes
    elements_static_size = realloc_esize;
    vertices_static_size = realloc_vsize;
    return vbo_static;
}

GLuint Renderer::add_dynamic_vertices(float* vertices, uint32_t length)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, length, vertices, GL_DYNAMIC_DRAW);
    return vbo;
}

GLuint Renderer::add_stream_vertices(float* vertices, uint32_t length)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, length, vertices, GL_STREAM_DRAW);
    return vbo;
}

int Renderer::add_shader(const std::string& name,
                         const std::string& vertex, 
                         const std::string& fragment,
                         bool raise_exception)
{
    static int shader_count = 0;
    shader_count++;
    std::unique_ptr<Shader> shader(new Shader(name, vertex, 
                                              fragment, raise_exception));
    shaders[shader_count] = std::move(shader);
    return shader_count;
}

int Renderer::add_fragment_shader(const std::string& name,
                         const std::string& fragment,
                         bool raise_exception)
{
    static int shader_count = 1000;
    shader_count++;
    std::unique_ptr<Shader> shader(new Shader(name, fragment, 
                Shader_type::FRAGMENT, raise_exception));
    shaders[shader_count] = std::move(shader);
    return shader_count;
}

int Renderer::add_vertex_shader(const std::string& name,
                                const std::string& vertex,
                                bool raise_exception)
{
    static int shader_count = 500;
    shader_count++;
    std::unique_ptr<Shader> shader(new Shader(name, vertex, 
                Shader_type::VERTEX, raise_exception));
    shaders[shader_count] = std::move(shader);
    return shader_count;
}


// TODO(justinvh): Do what this actually says
Shader* Renderer::get_or_create_shader(const std::string& name)
{
    std::stringstream vertex, fragment;
    vertex << "game/shaders/" << name << ".vert";
    fragment << "game/shaders/" << name << ".frag";
    string vertex_str = vertex.str();
    string fragment_str = fragment.str();
    int n = add_shader(name, vertex_str, fragment_str, true);
    return shaders[n].get();
}

Shader* Renderer::get_or_create_fragment_shader(const std::string& name)
{
    std::stringstream fragment;
    fragment << "game/shaders/" << name << ".frag";
    string fragment_str = fragment.str();
    int n = add_fragment_shader(name, fragment_str, true);
    return shaders[n].get();
}

Shader* Renderer::get_or_create_vertex_shader(const std::string& name)
{
    std::stringstream vertex;
    vertex << "game/shaders/" << name << ".vert";
    string vertex_str = vertex.str();
    int n = add_vertex_shader(name, vertex_str, true);
    return shaders[n].get();
}



void Renderer_tests::color_square()
{
    float vertices[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
    };

    int elements[] = {
        0, 1, 2,
        1, 3, 0
    };
    
    renderer->add_static_vertices(vertices, sizeof(vertices), 
                                  elements, sizeof(elements));
}

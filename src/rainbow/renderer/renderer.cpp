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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

void Renderer::run_frame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, total_elements, GL_UNSIGNED_INT, nullptr);
}

GLuint Renderer::add_static_vertices(const float* vertices, 
                                     uint32_t vlength,
                                     const int* elements,
                                     uint32_t elength)
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vlength, vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elength, elements, GL_STATIC_DRAW);

    total_vertices += vlength;
    total_elements += elength;

    return vbo;
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

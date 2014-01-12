#pragma once
#ifndef RAINBOW_RENDERER_SHADER_HPP
#define RAINBOW_RENDERER_SHADER_HPP

#include <sstream>
#include <exception>
#include <rainbow/gl.h>

namespace rb {

enum class Shader_type {
    VERTEX,
    FRAGMENT,
    PROGRAM,
    NOT_PROVIDED,
};

class bad_shader : public std::exception {
public:
    bad_shader(const std::string& file,
               const std::string& error,
               Shader_type shader_type)
        : type(shader_type), file(file), error(error) {
            std::stringstream full_msg;
            const char* type_name;
            type_name = type == Shader_type::VERTEX ? "vertex" : "fragment";
            full_msg << file << ": " << type_name << ": " << error << "\n";
            full_error = full_msg.str();
        }
    ~bad_shader() throw() = default;
public:
    const char* what() const throw() { return full_error.c_str(); }
    bool is_fragment() const throw() { return type == Shader_type::FRAGMENT; }
    bool is_vertex() const throw() { return type == Shader_type::VERTEX; }
    const Shader_type type;
    std::string file;
    std::string error;
    std::string full_error;
};

class Attribute {
public:
    Attribute() = default;
    Attribute(GLuint glprogram, const std::string& name);
    void eso(int elements, int stride, int offset,
             int size_of = sizeof(float), bool normalize = false);
    void vec2(int stride, int offset,
             int size_of = sizeof(float), bool normalize = false);
    void vec3(int stride, int offset,
             int size_of = sizeof(float), bool normalize = false);
    void vec4(int stride, int offset,
             int size_of = sizeof(float), bool normalize = false);
public:
    std::string name;
    GLuint glprogram;
    GLint attribute;
};

class Uniform {
public:
    Uniform() = default;
    Uniform(GLuint glprogram, const std::string& name);
    void int1(int value);
    void mat4(glm::mat4 matrix);
public:
    std::string name;
    GLuint glprogram;
    GLint uniform;
};

class Shader {
public:
    Shader() = default;
    Shader(const std::string& name,
            const std::string& file,
            Shader_type shader_type,
            bool raise_exception = false);
    Shader(const std::string& name,
           const std::string& vertex_file,
           const std::string& fragment_file,
           bool raise_exception = false);
    ~Shader();
    bool cleanup();
    bool initialize(const std::string& name);
    bool compile(const std::string& shader_file,
                 Shader_type shader_type = Shader_type::NOT_PROVIDED);
    bool attach(Shader_type which = Shader_type::NOT_PROVIDED);
    bool link();
    bool bind(const std::string& what, Shader_type where);
    bool use();
    bool program_initialized();
    Attribute attrib(const std::string& attribute);
    Uniform uniform(const std::string& uniform);
public:
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint glprogram;
    bool valid;
    char error[513];
    std::string name;
};

}

#endif // RAINBOW_RENDERER_SHADER_HPP

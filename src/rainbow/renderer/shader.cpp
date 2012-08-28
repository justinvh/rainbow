#include <rainbow/renderer/shader.hpp>
#include <rainbow/file.hpp>
#include <iostream>

using rb::Shader;
using rb::Attribute;
using rb::Uniform;

Shader::Shader(const std::string& name,
               const std::string& vertex_file, 
               const std::string& fragment_file, 
               bool raise_exception)
{
    initialize(name);

    if (!compile(vertex_file, Shader_type::VERTEX)) {
        if (raise_exception)
            throw bad_shader(vertex_file, error, Shader_type::VERTEX);
        return;
    }

    if (!compile(fragment_file, Shader_type::FRAGMENT)) {
        if (raise_exception)
            throw bad_shader(fragment_file, error, Shader_type::FRAGMENT);
        return;
    }

    if (!link()) {
        if (raise_exception)
            throw bad_shader(name, error, Shader_type::PROGRAM);
        return;
    }

    attach();
    link();

    valid = true;
}

Shader::~Shader()
{
    cleanup();
}

bool Shader::compile(const std::string& shader_file, Shader_type shader_type)
{
    GLint glshader_type;
    if (shader_type == Shader_type::FRAGMENT) {
        glshader_type = GL_FRAGMENT_SHADER;
    } else if (shader_type == Shader_type::VERTEX) {
        glshader_type = GL_VERTEX_SHADER;
    } else if (shader_type == Shader_type::NOT_PROVIDED) {
        std::string deduce = shader_file;
        std::string ext = deduce.substr(deduce.find_last_of(".") + 1);
        if (ext == "frag") {
            shader_type = Shader_type::FRAGMENT;
        } else if (ext == "vert") {
            shader_type = Shader_type::VERTEX;
        } else {
            sprintf(error, "%s", "shader extension could not be deduced");
            return false;
        }
    }

    rb::Contents contents = rb::read(shader_file);
    if (!contents.valid) {
        sprintf(error, "%s", contents.error.c_str());
        valid = false;
        return false;
    }

    std::string& shader_source = contents.data;
    const char* source = shader_source.c_str();

    GLint shader = glCreateShader(glshader_type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        memset(error, 0, sizeof(error));
        glGetShaderInfoLog(shader, 512, nullptr, error);
        valid = false;
        return false;
    }

    if (shader_type == Shader_type::FRAGMENT) {
        fragment_shader = shader;
    } else {
        vertex_shader = shader;
    }

    return true;
}

bool Shader::attach()
{
    if (glprogram == -1) {
        valid = false;
        sprintf(error, "%s", "program has not been created");
        return false;
    }

    glAttachShader(glprogram, vertex_shader);
    glAttachShader(glprogram, fragment_shader);
    return true;
}

bool Shader::bind(const std::string& what, Shader_type where)
{
    if (!program_initialized()) 
        return false;

    if (where == Shader_type::FRAGMENT) {
        glBindFragDataLocation(glprogram, 0, what.c_str());
    } else if (where == Shader_type::VERTEX) {
        glBindFragDataLocation(glprogram, 0, what.c_str());
    } else {
        valid = false;
        sprintf(error, "%s", "fragment or vertex shader must be specified");
        return false;
    }

    return true;
}

bool Shader::use()
{
    if (!program_initialized()) 
        return false;
    glUseProgram(glprogram);
    return true;
}

bool Shader::program_initialized()
{
    if (glprogram == -1) {
        valid = false;
        sprintf(error, "%s", "program has not been created");
        return false;
    }

    return true;
}

bool Shader::link()
{
    if (!program_initialized()) 
        return false;
    glLinkProgram(glprogram);
    return true;
}

bool Shader::initialize(const std::string& name)
{
    glprogram = glCreateProgram();
    this->name = name;
    return true;
}

bool Shader::cleanup()
{
    glDeleteProgram(glprogram);
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
    return true;
}

Attribute Shader::attrib(const std::string& attribute)
{
    return Attribute(glprogram, attribute);
}

Uniform Shader::uniform(const std::string& uniform)
{
    return Uniform(glprogram, uniform);
}


Attribute::Attribute(GLuint glprogram, const std::string& name)
    : name(name), glprogram(glprogram)
{
    attribute = glGetAttribLocation(glprogram, name.c_str());
}

void Attribute::eso(int elements, int stride, int offset, int size_of,
                    bool normalize)
{
    glEnableVertexAttribArray(attribute);
    glVertexAttribPointer(attribute, 
                          elements, 
                          GL_FLOAT, 
                          normalize ? GL_TRUE : GL_FALSE,
                          stride * size_of,
                          (void*)(offset * size_of));
}

void Attribute::vec2(int stride, int offset, int size_of, bool normalize)
{
    return eso(2, stride, offset, size_of, normalize);
}

void Attribute::vec3(int stride, int offset, int size_of, bool normalize)
{
    return eso(3, stride, offset, size_of, normalize);
}

Uniform::Uniform(GLuint glprogram, const std::string& name)
    : name(name), glprogram(glprogram)
{
    uniform = glGetUniformLocation(glprogram, name.c_str());
}

void Uniform::mat4(glm::mat4 matrix)
{
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
}

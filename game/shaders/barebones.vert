#version 150

in vec3 color;
in vec2 position;

out vec3 Color;

uniform mat4 trans;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
    Color = color;
    gl_Position = trans * vec4(position, 0.0, 2.0);
}

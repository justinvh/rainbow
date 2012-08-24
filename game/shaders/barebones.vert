#version 150

in vec3 color;
in vec2 position;

out vec3 Color;

uniform mat4 trans;

void main()
{
    Color = color;
    gl_Position = trans * vec4(position, 0.0, 1.0);
}

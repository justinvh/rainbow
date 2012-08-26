#version 150

in vec3 color;
in vec3 position;

out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    Color = color;
    gl_Position = proj * view * model * vec4(position, 10.0);
}

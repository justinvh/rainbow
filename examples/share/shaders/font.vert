#version 150

in vec3 color;
in vec3 position;
in vec4 texcoord;

out vec4 Texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    Texcoord = texcoord;
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}

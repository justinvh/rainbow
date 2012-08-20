#version 150

in vec3 color;
in vec2 position;
out vec3 Color;

void main()
{
    Color = color;
    gl_Position = vec4( position, 0.0, 1.0 );
}

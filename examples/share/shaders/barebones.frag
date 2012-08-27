#version 150

in vec3 Color;
out vec4 outColor;

void main()
{
    outColor = vec4(Color.r, Color.r, Color.r, 1.0);
}

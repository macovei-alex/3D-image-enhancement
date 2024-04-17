#version 330 core

in vec3 MidColor;

out vec4 OutColor;

void main()
{
    OutColor = vec4(MidColor, 1.0f);
}
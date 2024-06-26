#version 330 core

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec3 InColor;

out vec3 MidColor;

void main()
{
    gl_Position = vec4(InPosition, 1.0f);
    MidColor = InColor;
}
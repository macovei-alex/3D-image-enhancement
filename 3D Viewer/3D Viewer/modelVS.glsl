#version 330 core

layout (location = 0) in vec4 InPosition;
layout (location = 1) in vec4 InColor;

out vec4 MidColor;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * InPosition;
    MidColor = InColor;
}
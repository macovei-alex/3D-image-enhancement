#version 330 core

layout(location = 0) in vec4 InPosition;
layout(location = 1) in vec3 InNormal;

out vec3 MidFragmentPosition;
out vec3 MidNormal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	MidFragmentPosition = vec3(ModelMatrix * InPosition);
	MidNormal = mat3(transpose(inverse(ModelMatrix))) * InNormal;

	gl_Position = ProjectionMatrix * ViewMatrix * vec4(MidFragmentPosition, 1.0);
}
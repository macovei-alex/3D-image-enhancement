#version 330 core

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec3 InColor;
layout (location = 2) in vec3 InNormal;

out vec3 MidFragmentPosition;
out vec3 MidColor;
out vec3 MidNormal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	MidFragmentPosition = vec3(ModelMatrix * vec4(InPosition, 1.0f));
	MidNormal = mat3(transpose(inverse(ModelMatrix))) * InNormal;

	gl_Position = ProjectionMatrix * ViewMatrix * vec4(MidFragmentPosition, 1.0);
	MidColor = InColor;
}
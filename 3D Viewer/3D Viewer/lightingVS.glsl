#version 330 core

layout(location = 0) in vec3 AbsolutePosition;
layout(location = 1) in vec3 AbsoluteNormal;

out vec3 FragmentPosition;
out vec3 Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	FragmentPosition = vec3(ModelMatrix * vec4(AbsolutePosition, 1.0));
	Normal = mat3(transpose(inverse(ModelMatrix))) * AbsoluteNormal;

	gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragmentPosition, 1.0);
}
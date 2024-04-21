#version 330 core

in vec3 MidFragmentPosition;
in vec3 MidColor;
in vec3 MidNormal;

out vec4 OutFragmentColor;

uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 ViewPosition;

uniform float AmbientStrength;
uniform float DiffuseStrength;
uniform float SpecularStrength;
uniform int SpecularExponent;

void main()
{
	vec3 ambient = AmbientStrength * LightColor;

	vec3 lightDirection = normalize(LightPosition - MidFragmentPosition);
	float diffuseValue = max(dot(MidNormal, lightDirection), 0.0);
	vec3 diffuse = DiffuseStrength * diffuseValue * LightColor;

    vec3 viewDirection = normalize(ViewPosition - MidFragmentPosition);
	vec3 reflectionDirection = reflect(-lightDirection, MidNormal);
	float specularPower = pow(max(dot(viewDirection, reflectionDirection), 0.0), SpecularExponent);
	vec3 specular = SpecularStrength * specularPower * LightColor;

    vec3 result = (ambient + diffuse + specular) * MidColor;

	OutFragmentColor = vec4(result, 1.0);
}
#version 330 core

in vec3 Normal;
in vec3 FragmentPosition;

out vec4 FragmentColor;

uniform vec3 LightPosition;
uniform vec3 ViewPosition;
uniform vec3 LightColor;
uniform vec3 ObjectColor;

uniform float AmbientStrength;
uniform float DiffuseStrength;
uniform float SpecularStrength;
uniform int SpecularExponent;

void main()
{
	vec3 ambient = AmbientStrength * LightColor;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(LightPosition - FragmentPosition);
	float diffuseValue = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = DiffuseStrength * diffuseValue * LightColor;

    vec3 viewDirection = normalize(ViewPosition - FragmentPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularPower = pow(max(dot(viewDirection, reflectionDirection), 0.0), SpecularExponent);
	vec3 specular = SpecularStrength * specularPower * LightColor;

    vec3 result = (ambient + diffuse + specular) * ObjectColor;

	FragmentColor = vec4(result, 1.0);
}
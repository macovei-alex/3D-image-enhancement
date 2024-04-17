#pragma once

#include "Model.h"

class LightSource
{
public:
	LightSource(const Model& model);
	LightSource(Model&& model);

	Model& GetModel();
	float GetAmbientStrength() const;
	float GetSpecularStrength() const;
	float GetDiffuseStrength() const;
	int GetSpecularExponent() const;
	glm::vec3 GetColor() const;

	void SetAmbientStrength(float ambientStrength);
	void SetSpecularStrength(float specularStrength);
	void SetDiffuseStrength(float diffuseStrength);
	void SetSpecularExponent(int specularExponent);
	void SetColor(const glm::vec3& color);

private:
	Model model;

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	float ambientStrength = 0.3f;
	float specularStrength = 0.5f;
	float diffuseStrength = 0.5f;
	int specularExponent = 32;
};

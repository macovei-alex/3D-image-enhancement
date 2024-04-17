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

	void SetAmbientStrength(float ambientStrength);
	void SetSpecularStrength(float specularStrength);
	void SetDiffuseStrength(float diffuseStrength);
	void SetSpecularExponent(int specularExponent);

private:
	Model model;

	float ambientStrength = 0.1f;
	float specularStrength = 0.5f;
	float diffuseStrength = 0.5f;
	int specularExponent = 32;
};

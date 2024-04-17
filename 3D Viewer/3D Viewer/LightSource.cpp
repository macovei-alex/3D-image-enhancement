#include "LightSource.h"

LightSource::LightSource(const Model& model)
	: model(model)
{
	// empty
}

LightSource::LightSource(Model&& model)
	: model(std::move(model))
{
	// empty
}

Model& LightSource::GetModel()
{
	return model;
}

float LightSource::GetAmbientStrength() const
{
	return ambientStrength;
}

float LightSource::GetSpecularStrength() const
{
	return specularStrength;
}

float LightSource::GetDiffuseStrength() const
{
	return diffuseStrength;
}

int LightSource::GetSpecularExponent() const
{
	return specularExponent;
}

void LightSource::SetAmbientStrength(float ambientStrength)
{
	this->ambientStrength = ambientStrength;
}

void LightSource::SetSpecularStrength(float specularStrength)
{
	this->specularStrength = specularStrength;
}

void LightSource::SetDiffuseStrength(float diffuseStrength)
{
	this->diffuseStrength = diffuseStrength;
}

void LightSource::SetSpecularExponent(int specularExponent)
{
	this->specularExponent = specularExponent;
}

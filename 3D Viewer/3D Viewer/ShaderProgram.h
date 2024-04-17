#pragma once

#include "Utils.h"

#include <string>

#include <glm.hpp>

class ShaderProgram
{
public:
	ShaderProgram() = delete;
	ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
	~ShaderProgram();

	void Use() const;

	GLuint GetID() const;

	void SetInt(const std::string& locationName, int value) const;
	void SetFloat(const std::string& locationName, float value) const;
	void SetVec3(const std::string& locationName, const glm::vec3& value) const;
	void SetMat4(const std::string& locationName, const glm::mat4& mat) const;

private:
	void Init(const std::string& vertexPath, const std::string& fragmentPath);
	void CheckCompileErrors(GLuint shaderStencilTesting, const std::string& type);

private:
	GLuint ID;

	GLuint modelMatrixLocation;
	GLuint viewMatrixLocation;
	GLuint projectionMatrixLocation;
};
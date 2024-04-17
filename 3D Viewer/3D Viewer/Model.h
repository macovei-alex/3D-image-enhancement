#pragma once

#include <string>
#include <vector>
#include <glm.hpp>
#include <gl/glew.h>

#include "Utils.h"

class Model
{
public:
	Model(const std::string& filePath, bool makeCentered);
	Model(Model&& model) noexcept;
	Model(const Model&);
	~Model();

	void Render() const;
	void CenterModel();

	glm::mat4 GetModelMatrix() const;
	glm::vec3 GetPosition() const;

	void SetPosition(const glm::vec3& position);
	void SetScale(const glm::vec3& scale);
	void SetRotation(const glm::vec3& rotation);
	void Translate(const glm::vec3& translation);
	void Rotate(const glm::vec3& rotation);
	void Scale(const glm::vec3& scale);

private:
	void ReadVertices(std::ifstream& fin);
	void ReadColors(std::ifstream& fin);
	void ReadIndices(std::ifstream& fin);

	void CalculateNormals();

	void InitBuffers();
	void DestroyBuffers();

private:
	glm::mat4 modelMatrix;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3u> indices;
	std::vector<glm::vec3> normals;

	GLuint vertexArrayID, vertexBufferID, colorBufferID, indexBufferID, normalBufferID;

	bool isCentered;
};

#pragma once

#include "utils.h"
#include "Vertex.h"

class Model
{
public:
	Model(const std::string& filePath);
	Model(Model&& model) noexcept;
	Model(const Model&);
	~Model();

	void Render() const;

	glm::mat4 GetModelMatrix() const;
	glm::vec3 GetPosition() const;

	void SetPosition(const glm::vec3& position);
	void SetScale(const glm::vec3& scale);
	void SetRotation(const glm::vec3& rotation);

	void Translate(const glm::vec3& translation);
	void Scale(const glm::vec3& scale);
	void Rotate(const glm::vec3& rotation);

	void CenterModel();

private:
	void ReadVertices(std::istream& fin);
	void ReadIndices(std::istream& fin);

	void CalculateNormals();

	void InitBuffers();
	void DestroyBuffers();

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	GLuint VAO, VBO, EBO;

	glm::mat4 modelMatrix;
};
#pragma once

#include <string>
#include <vector>
#include <glm.hpp>
#include <gl/glew.h>

typedef glm::vec4 Vertex;
typedef glm::vec4 Color;
typedef glm::vec<3, unsigned int> Index;
typedef glm::vec3 Normal;

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

private:
	void ReadVertices(std::ifstream& fin);
	void ReadColors(std::ifstream& fin);
	void ReadIndices(std::ifstream& fin);

	void CalculateNormals();

	void InitBuffers();

private:
	glm::mat4 modelMatrix;

	std::vector<Vertex> vertices;
	std::vector<Color> colors;
	std::vector<Index> indices;
	std::vector<Normal> normals;

	GLuint vertexArrayID, vertexBufferID, colorBufferID, indexBufferID, normalBufferID;
};

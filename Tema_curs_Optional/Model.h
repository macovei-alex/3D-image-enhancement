#pragma once

#include <string>
#include <vector>
#include <glm.hpp>
#include <gl/glew.h>

typedef glm::vec4 Vertex;
typedef glm::vec4 Color;
typedef glm::vec<3, unsigned int> Index;

class Model
{
public:
	Model(const std::string& filePath);
	~Model();

	glm::mat4 GetModelMatrix() const;

	void Render() const;

private:
	void ReadVertices(std::ifstream& fin);
	void ReadColors(std::ifstream& fin);
	void ReadIndices(std::ifstream& fin);

	void InitBuffers();

private:
	glm::mat4 modelMatrix;

	std::vector<Vertex> vertices;
	std::vector<Color> colors;
	std::vector<Index> indices;

	GLuint vertexArrayID, vertexBufferID, colorBufferID, indexBufferID;
};

#include "Model.h"

#include <fstream>
#include <gtc/matrix_transform.hpp>

Model::Model(const std::string& filePath, bool makeCentered)
	: modelMatrix(glm::mat4(1.0f))
{
	std::ifstream fin(filePath);
	std::string line;

	ReadVertices(fin);
	ReadColors(fin);
	ReadIndices(fin);

	if (colors.size() < vertices.size())
		colors.insert(colors.end(), vertices.size() - colors.size(), glm::vec3(1.0f));
	else if (colors.size() > vertices.size())
		colors.resize(vertices.size());

	isCentered = false;
	if (makeCentered)
		CenterModel();

	CalculateNormals();
	InitBuffers();
}

Model::Model(Model&& model) noexcept
	: vertices(std::move(model.vertices)), colors(std::move(model.colors)), indices(std::move(model.indices)), modelMatrix(std::move(model.modelMatrix))
{
	vertexArrayID = model.vertexArrayID;
	vertexBufferID = model.vertexBufferID;
	colorBufferID = model.colorBufferID;
	indexBufferID = model.indexBufferID;
	normalBufferID = model.normalBufferID;
	isCentered = model.isCentered;

	model.vertexArrayID = 0;
	model.vertexBufferID = 0;
	model.colorBufferID = 0;
	model.indexBufferID = 0;
	model.normalBufferID = 0;
}

Model::Model(const Model& model)
	: vertices(model.vertices), colors(model.colors), indices(model.indices), modelMatrix(model.modelMatrix)
{
	InitBuffers();
}

Model::~Model()
{
	DestroyBuffers();
}

void Model::CenterModel()
{
	glm::vec3 center = glm::vec3(0.0f);
	for(const auto& vertex : vertices)
		center += vertex;

	center /= vertices.size();

	for (auto & vertex : vertices)
		vertex -= center;
}

glm::mat4 Model::GetModelMatrix() const
{
	return modelMatrix;
}

glm::vec3 Model::GetPosition() const
{
	return glm::vec3(modelMatrix[0][3], modelMatrix[1][3], modelMatrix[2][3]);
}

void Model::SetPosition(const glm::vec3& position)
{
	modelMatrix[0][3] = position.x;
	modelMatrix[1][3] = position.y;
	modelMatrix[2][3] = position.z;
}

void Model::SetScale(const glm::vec3& scale)
{
	modelMatrix[0][0] = scale.x;
	modelMatrix[1][1] = scale.y;
	modelMatrix[2][2] = scale.z;
}

void Model::Translate(const glm::vec3& translation)
{
	modelMatrix = glm::translate(modelMatrix, translation);
}

void Model::Scale(const glm::vec3& scale)
{
	modelMatrix = glm::scale(modelMatrix, scale);
}

void Model::Rotate(const glm::vec3& radians)
{
	modelMatrix = glm::translate(modelMatrix, -GetPosition());

	modelMatrix = glm::rotate(modelMatrix, radians.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, radians.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, radians.z, glm::vec3(0.0f, 0.0f, 1.0f));

	modelMatrix = glm::translate(modelMatrix, GetPosition());
}

void Model::ReadVertices(std::ifstream& fin)
{
	int vertexCount;
	fin >> vertexCount;
	for (int i = 0; i < vertexCount; i++)
	{
		float x, y, z;
		fin >> x >> y >> z;
		vertices.emplace_back(x, y, z);
	}
}

void Model::ReadColors(std::ifstream& fin)
{
	int colorCount;
	fin >> colorCount;
	for (int i = 0; i < colorCount; i++)
	{
		float r, g, b;
		fin >> r >> g >> b;
		colors.emplace_back(r, g, b);
	}
}

void Model::ReadIndices(std::ifstream& fin)
{
	int indexCount;
	fin >> indexCount;
	for (int i = 0; i < indexCount; i++)
	{
		unsigned int i1, i2, i3;
		fin >> i1 >> i2 >> i3;
		indices.emplace_back(i1, i2, i3);
	}
}

void Model::CalculateNormals()
{
	normals.resize(vertices.size());

	for (auto& index : indices)
	{
		normals[index[0]] += glm::cross(
			glm::vec3(vertices[index[1]] - vertices[index[0]]),
			glm::vec3(vertices[index[2]] - vertices[index[0]]));
		normals[index[1]] += glm::cross(
			glm::vec3(vertices[index[2]] - vertices[index[1]]),
			glm::vec3(vertices[index[0]] - vertices[index[1]]));
		normals[index[2]] += glm::cross(
			glm::vec3(vertices[index[0]] - vertices[index[2]]),
			glm::vec3(vertices[index[1]] - vertices[index[2]]));
	}

	for (auto& normal : normals)
	{
		if (glm::length(normal) > 0.0f)
			normal = glm::normalize(normal);
	}
}

void Model::InitBuffers()
{
	// Generate and bind VAO
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// Generate and bind vertex buffer
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, sizeof(vertices[0]), GL_FLOAT, GL_FALSE, 0, 0);

	// Generate and bind color buffer
	glGenBuffers(1, &colorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors[0]), colors.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, sizeof(colors[0]), GL_FLOAT, GL_FALSE, 0, 0);

	// Generate and bind normal buffer
	glGenBuffers(1, &normalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, sizeof(normals[0]), GL_FLOAT, GL_FALSE, 0, 0);

	// Generate and bind index buffer
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

	// Unbind VAO
	glBindVertexArray(0);
}

void Model::DestroyBuffers()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (colorBufferID != 0)
		glDeleteBuffers(1, &colorBufferID);
	if (normalBufferID != 0)
		glDeleteBuffers(1, &normalBufferID);
	if (indexBufferID != 0)
		glDeleteBuffers(1, &indexBufferID);
	if (vertexBufferID != 0)
		glDeleteBuffers(1, &vertexBufferID);

	if (vertexArrayID != 0)
		glDeleteVertexArrays(1, &vertexArrayID);

	colorBufferID = 0;
	normalBufferID = 0;
	indexBufferID = 0;
	vertexBufferID = 0;
	vertexArrayID = 0;
}

void Model::Render() const
{
	glBindVertexArray(vertexArrayID);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(0, sizeof(vertices[0]), GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glVertexAttribPointer(1, sizeof(colors[0]), GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glVertexAttribPointer(2, sizeof(normals[0]), GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glDrawElements(GL_TRIANGLES, indices.size() * sizeof(indices[0]), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
}

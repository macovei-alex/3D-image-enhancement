#include "Model.h"

#include "Utils.h"

#include <fstream>

Model::Model(const std::string& filePath, bool makeCentered)
	: modelMatrix(glm::mat4(1.0f)), isCentered(false)
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

	if (makeCentered)
		CenterModel();

	CalculateNormals();
	InitBuffers();
}

Model::Model(Model&& model) noexcept
	: vertices(std::move(model.vertices)), colors(std::move(model.colors)), indices(std::move(model.indices)), normals(std::move(model.normals)), modelMatrix(std::move(model.modelMatrix))
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
	model.isCentered = false;
}

Model::Model(const Model& model)
	: vertices(model.vertices), colors(model.colors), indices(model.indices), normals(model.normals), modelMatrix(model.modelMatrix)
{
	InitBuffers();
	isCentered = model.isCentered;
}

Model::~Model()
{
	DestroyBuffers();
}

glm::mat4 Model::GetModelMatrix() const
{
	return modelMatrix;
}

glm::vec3 Model::GetPosition() const
{
	return glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
}

void Model::SetPosition(const glm::vec3& position)
{
	modelMatrix[3][0] = position.x;
	modelMatrix[3][1] = position.y;
	modelMatrix[3][2] = position.z;
}

void Model::SetScale(const glm::vec3& scale)
{
	modelMatrix[0][0] = scale.x;
	modelMatrix[1][1] = scale.y;
	modelMatrix[2][2] = scale.z;
}

void Model::SetRotation(const glm::vec3& rotation)
{
	modelMatrix = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Model::Translate(const glm::vec3& translation)
{
	modelMatrix = glm::translate(modelMatrix, translation);
}

void Model::Scale(const glm::vec3& scale)
{
	modelMatrix = glm::scale(modelMatrix, scale);
}

void Model::Rotate(const glm::vec3& rotation)
{
	modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Model::CenterModel()
{
	if (isCentered)
		return;

	DestroyBuffers();

	glm::vec3 center = glm::vec3(0.0f);
	for (glm::vec3& vertex : vertices)
		center += vertex;

	center /= vertices.size();

	for (glm::vec3& vertex : vertices)
		vertex -= center;

	isCentered = true;
	InitBuffers();
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
		glm::vec3 v1 = glm::vec3(vertices[index[1]] - vertices[index[0]]);
		glm::vec3 v2 = glm::vec3(vertices[index[2]] - vertices[index[0]]);

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
	// se creeaza un buffer nou
	glGenBuffers((int)Count::One, &vertexBufferID);
	// este setat ca buffer curent
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	// punctele sunt "copiate" in bufferul curent
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

	// se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
	glGenVertexArrays((int)Count::One, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// se activeaza lucrul cu atribute; atributul 0 = pozitie
	glEnableVertexAttribArray((int)AtribNumber::Zero);
	glVertexAttribPointer((int)Layout::Location_0, (int)StepSize::Float, GL_FLOAT, GL_FALSE, (int)SpaceBetween::Zero, (void*)StartOffset::Zero);

	// un nou buffer, pentru culoare
	glGenBuffers((int)Count::One , &colorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors[0]), colors.data(), GL_STATIC_DRAW);

	// atributul 1 = culoare
	glEnableVertexAttribArray((int)AtribNumber::One);
	glVertexAttribPointer((int)Layout::Location_1, (int)StepSize::Float, GL_FLOAT, GL_FALSE, (int)SpaceBetween::Zero, (void*)StartOffset::Zero);

	glGenBuffers((int)Count::One, &normalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray((int)AtribNumber::Two);
	glVertexAttribPointer((int)Layout::Location_2, (int)StepSize::Float, GL_FLOAT, GL_FALSE, (int)SpaceBetween::Zero, (void*)StartOffset::Zero);

	// un nou buffer pentru indexuri
	glGenBuffers((int)Count::One, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::Render() const
{
	glBindVertexArray(vertexArrayID);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glDrawElements(GL_TRIANGLES, indices.size() * sizeof(glm::vec3u), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
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
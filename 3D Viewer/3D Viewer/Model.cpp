#include "Model.h"

#include <fstream>

#include "Utils.h"

Model::Model(const std::string& filePath)
	: modelMatrix(glm::mat4(1.0f))
{
	std::ifstream fin(filePath);
	std::string line;

	ReadVertices(fin);
	ReadColors(fin);
	ReadIndices(fin);

	if (colors.size() < vertices.size())
	{
		colors.insert(colors.end(), vertices.size() - colors.size(), Color(1.0f));
	}
	else if (colors.size() > vertices.size())
	{
		colors.resize(vertices.size());
	}

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
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &colorBufferID);
	glDeleteBuffers(1, &vertexBufferID);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vertexArrayID);
}

glm::mat4 Model::GetModelMatrix() const
{
	return modelMatrix;
}

glm::vec3 Model::GetPosition() const
{
	return glm::vec3(modelMatrix[3]);
}

void Model::SetPosition(const glm::vec3& position)
{
	modelMatrix[3] = glm::vec4(position, 1.0f);
}

void Model::ReadVertices(std::ifstream& fin)
{
	int vertexCount;
	fin >> vertexCount;
	for (int i = 0; i < vertexCount; i++)
	{
		float x, y, z;
		fin >> x >> y >> z;
		vertices.emplace_back(x, y, z, 1.0f);
	}
}

void Model::ReadColors(std::ifstream& fin)
{
	int colorCount;
	fin >> colorCount;
	for(int i = 0; i < colorCount; i++)
	{
		float r, g, b;
		fin >> r >> g >> b;
		colors.emplace_back(r, g, b, 1.0f);
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

	for (Index& index : indices)
	{
		glm::vec3 v1 = glm::vec3(vertices[index[1]] - vertices[index[0]]);
		glm::vec3 v2 = glm::vec3(vertices[index[2]] - vertices[index[0]]);

		normals[index[0]] += glm::cross(
			glm::vec3(vertices[index[1]] - vertices[index[0]]),
			glm::vec3(vertices[index[2]] - vertices[index[0]]));
	}

	for (Normal& normal : normals)
	{
		if(glm::length(normal) > 0.0f)
			normal = glm::normalize(normal);
	}
}

void Model::InitBuffers()
{
	// se creeaza un buffer nou
	glGenBuffers(1, &vertexBufferID);
	// este setat ca buffer curent
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	// punctele sunt "copiate" in bufferul curent
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// se activeaza lucrul cu atribute; atributul 0 = pozitie
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(Layout::Location_0, sizeof(float), GL_FLOAT, GL_FALSE, 0, 0);

	// un nou buffer, pentru culoare
	glGenBuffers(1, &colorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Color), colors.data(), GL_STATIC_DRAW);

	// atributul 1 = culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(Layout::Location_1, sizeof(float), GL_FLOAT, GL_FALSE, 0, 0);

	// un nou buffer pentru indexuri
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Index), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &normalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Normal), normals.data(), GL_STATIC_DRAW);

	// atributul 2 = normala
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(Layout::Location_2, sizeof(float), GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::Render() const
{
	glBindVertexArray(vertexArrayID);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), (void*)0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glDrawElements(GL_TRIANGLES, indices.size() * sizeof(Index), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
}

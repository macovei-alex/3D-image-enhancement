#include "Model.h"

#include "utils.h"

Model::Model(const std::string& filePath)
	: modelMatrix(glm::mat4(1.0f))
{
	std::ifstream fin(filePath);
	std::string line;

	ReadVertices(fin);
	ReadIndices(fin);

	if (indices.size() % 3 != 0)
	{
		throw std::exception(std::format("The number of indices is not divisible by 3 in model file {}: {}", filePath, indices.size()).data());
	}

	VAO = 0;
	VBO = 0;
	EBO = 0;
	
	CenterModel();

	CalculateNormals();
	InitBuffers();
}

Model::Model(Model&& model) noexcept
	: vertices(std::move(model.vertices)), indices(std::move(model.indices)), modelMatrix(std::move(model.modelMatrix))
{
	VAO = model.VAO;
	VBO = model.VBO;
	EBO = model.EBO;

	model.VAO = 0;
	model.VBO = 0;
	model.EBO = 0;
}

Model::Model(const Model& model)
	: vertices(model.vertices), modelMatrix(model.modelMatrix)
{
	InitBuffers();
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
	glm::vec3 center = glm::vec3(0.0f);
	for (auto& vertex : vertices)
		center += vertex.position;

	center /= vertices.size();

	for (auto& vertex : vertices)
		vertex.position -= center;
}

void Model::ReadVertices(std::istream& fin)
{
	int vertexCount;
	fin >> vertexCount;

	vertices.resize(vertexCount);

	for (int i = 0; i < vertexCount; i++)
	{
		float x, y, z;
		fin >> x >> y >> z;
		vertices[i].position = glm::vec3(x, y, z);
	}

	int colorCount;
	fin >> colorCount;
	for (int i = 0; i < colorCount; i++)
	{
		float r, g, b;
		fin >> r >> g >> b;
		vertices[i].color = glm::vec3(r, g, b);
	}

	/*
	for (int i = 0; i < vertexCount; i++)
	{
		float nx, ny, nz;
		fin >> nx >> ny >> nz;
		vertices[i].normal = glm::vec3(nx, ny, nz);
	}
	*/
}

void Model::ReadIndices(std::istream& fin)
{
	unsigned int indexCount;
	fin >> indexCount;

	indices.resize(indexCount * 3);

	for (unsigned int i = 0; i < indexCount * 3; i++)
	{
		unsigned int index;
		fin >> index;
		indices[i] = index;
	}
}

void Model::CalculateNormals()
{
	for (int i = 0; i < indices.size(); i += 3)
	{
		glm::vec3 v1 = vertices[indices[i + 1]].position - vertices[indices[i]].position;
		glm::vec3 v2 = vertices[indices[i + 2]].position - vertices[indices[i + 1]].position;
		glm::vec3 v3 = vertices[indices[i]].position - vertices[indices[i + 2]].position;

		vertices[indices[i]].normal += glm::cross(v1, -v3);
		vertices[indices[i + 1]].normal += glm::cross(v2, -v1);
		vertices[indices[i + 2]].normal += glm::cross(v3, -v2);
	}

	for (auto& vertex : vertices)
	{
		if (glm::length(vertex.normal) > 0.0f)
			vertex.normal = glm::normalize(vertex.normal);
	}
}

void Model::InitBuffers()
{
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glBindVertexArray(VAO));

	GLCall(glGenBuffers(1, &VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW));

	// vertex Positions
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, dimof(vertices[0].position), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)));

	// vertex normals
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, dimof(vertices[0].normal), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));

	// vertex color coords
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, dimof(vertices[0].color), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color)));

	GLCall(glGenBuffers(1, &EBO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));
}

void Model::Render() const
{
	GLCall(glBindVertexArray(VAO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));

	GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));
}

void Model::DestroyBuffers()
{
	GLCall(glBindVertexArray(0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	if(EBO != 0)
		GLCall(glDeleteBuffers(1, &EBO));
	if(VBO != 0)
		GLCall(glDeleteBuffers(1, &VBO));
	if(VAO != 0)
		GLCall(glDeleteVertexArrays(1, &VAO));

	VAO = 0;
	VBO = 0;
	EBO = 0;
}
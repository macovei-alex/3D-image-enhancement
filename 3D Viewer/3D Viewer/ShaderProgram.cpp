#include "ShaderProgram.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Utils.h"
#include <GL/glew.h>

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath)
{
	Init(vertexPath, fragmentPath);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(ID);
}

void ShaderProgram::Use() const
{
	glUseProgram(ID);
}

GLuint ShaderProgram::GetID() const
{
	return ID;
}

void ShaderProgram::SetInt(const std::string& locationName, int value) const
{
	glUniform1i(glGetUniformLocation(ID, locationName.c_str()), value);
}

void ShaderProgram::SetFloat(const std::string& locationName, float value) const
{
	glUniform1f(glGetUniformLocation(ID, locationName.c_str()), value);
}

void ShaderProgram::SetVec3(const std::string& locationName, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, locationName.c_str()), 1, &value[0]);
}

void ShaderProgram::SetMat4(const std::string& locationName, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, locationName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::Init(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFileStream;
	std::ifstream fShaderFileStream;

	vShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFileStream.open(vertexPath);
		fShaderFileStream.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFileStream.rdbuf();
		fShaderStream << fShaderFileStream.rdbuf();

		vShaderFileStream.close();
		fShaderFileStream.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure exception)
	{
		std::cout << "ERROR when reading the shaders: " << exception.what() << std::endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void ShaderProgram::CheckCompileErrors(GLuint shaderStencilTesting, const std::string& type)
{
	GLint success;
	GLchar infoLog[1024];

	if (type != "PROGRAM")
	{
		glGetShaderiv(shaderStencilTesting, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderStencilTesting, 1024, NULL, infoLog);
			std::cout << "ERROR when compiling shader of type: " << type << "\n"
				<< infoLog << "\n ---------------------------------------------------"
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(shaderStencilTesting, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderStencilTesting, 1024, NULL, infoLog);
			std::cout << "ERROR when linking program of type: " << type << "\n"
				<< infoLog << "\n ---------------------------------------------------"
				<< std::endl;
		}
	}
}
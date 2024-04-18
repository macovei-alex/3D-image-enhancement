#pragma once

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

namespace glm
{
	typedef glm::vec<3, unsigned int> vec3u;
}

enum class Layout : GLuint
{
	Location_0,
	Location_1,
	Location_2
};

enum class AtribNumber : GLuint
{
	Zero,
	One,
	Two
};

enum class Count : GLint
{
	Zero,
	One,
	Two
};

enum class StepSize : GLint
{
	Int = sizeof(int),
	UInt = sizeof(unsigned int),
	Float = sizeof(float),
};

enum class SpaceBetween : GLint
{
	Zero = 0
};

enum class StartOffset : GLint
{
	Zero = 0
};

std::ostream& operator<<(std::ostream& os, const glm::vec3& vector);
std::ostream& operator<<(std::ostream& os, const glm::mat4& mat);
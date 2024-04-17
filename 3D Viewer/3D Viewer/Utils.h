#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <iostream>

namespace glm 
{
	typedef glm::vec<3, unsigned int> vec3u;
}
enum Layout : GLenum
{
	Location_0,
	Location_1,
	Location_2
};

enum Count : GLenum
{
	Zero,
	One,
	Two
};

std::ostream& operator<<(std::ostream& os, const glm::vec3& vector);
std::ostream& operator<<(std::ostream& os, const glm::mat4& mat);

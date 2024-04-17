#pragma once

#include <GL/glew.h>
#include <glm.hpp>

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

namespace glm
{
	typedef glm::vec<3, unsigned int> vec3u;
}

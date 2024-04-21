#include "utils.h"

std::ostream& operator<<(std::ostream& os, const glm::vec3& vector)
{
	os << vector.x << ' ' << vector.y << ' ' << vector.z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::mat4& mat)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			os << mat[j][i] << ' ';
		os << '\n';
	}
	return os;
}

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	GLenum error;
	bool ret = true;
	while ((error = glGetError()) != GL_NO_ERROR)
	{
		std::stringstream ss;
		ss << std::hex << error;
		std::string errorHex = ss.str();

		std::cout << std::format("[OpenGL Error]:\n\t code ( 0x{} )\n\t in ( {} )\n\t at ( {} )\n\t during ( {} )\n", errorHex, file, line, function);
		ret = false;
	}
	return ret;
}

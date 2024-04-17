#include "Utils.h"

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
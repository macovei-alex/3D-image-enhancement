#include "Utils.h"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
	os << vec.x << ' ' << vec.y << ' ' << vec.z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::mat4& mat)
{
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
			std::cout << mat[i][j] << ' ';
		std::cout << '\n';
	}
	return os;
}

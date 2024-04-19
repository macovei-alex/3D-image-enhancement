#pragma once

#include "Utils.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;

	Vertex() : position(0.0f), normal(0.0f), color(0.0f) {}
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& color)
		: position(position), normal(normal), color(color) {}
};

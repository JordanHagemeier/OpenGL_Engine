#pragma once

#include <glm/glm.hpp>

class Vertex {
public:
	glm::vec3 position	= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec2 uvs		= glm::vec2(0.0f, 0.0f);
	glm::vec3 normals	= glm::vec3(0.0f, 0.0f, 0.0f);

	const float* data() const {
		return &(position.x);
	}
};

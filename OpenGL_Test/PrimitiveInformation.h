#pragma once
#include "glm/glm.hpp"
#include "Serialization/JsonArchive.h"

struct PrimitiveInformation {

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	float radius = 0.0f;
	float donutRadius = 0.0f;
	int points = 0;
	int donutPoints = 0;
	float ringThickness = 0.0f;

	void Serialize(JsonSerializationArchive& archive) {
		archive.Serialize(position.x, "position.x");
		archive.Serialize(position.y, "position.y");
		archive.Serialize(position.z, "position.z");
		archive.Serialize(radius, "radius");
		archive.Serialize(donutRadius, "donutRadius");
		archive.Serialize(points, "points");
		archive.Serialize(donutPoints, "donutPoints");
		archive.Serialize(ringThickness, "ringThickness");

	}

};


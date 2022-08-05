#include "CubeController.h"
#include "Serialization/JsonArchive.h"

void CubeControllerComponent::Serialize(JsonSerializationArchive& archive)
{
	IComponent::Serialize(archive);

	archive.Serialize(testValue, "CubeTestValue");
}

void CubeControllerComponent::Update() {

	float offset = 1.0f;
	float radius = 4.0f;
	glm::vec3 midPoint(0.0f, 0.0f, 0.0f);


	float radian = (2 * glm::pi<float>() * ((float)glfwGetTime() / 5) + glm::radians(offset));

	float cos_rad = glm::cos(radian);
	float sin_rad = glm::sin(radian);

	float newX = (midPoint.x + cos_rad) * radius;
	float newY = (midPoint.y + sin_rad) * radius;

	glm::vec3 newPosition(newX, newY, 0.0f);

	m_ParentGameObject->setTransform(newPosition);

}
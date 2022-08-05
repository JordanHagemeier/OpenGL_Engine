#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	Camera();
	Camera(glm::vec3, glm::vec3, glm::vec3);

	alignas(16) glm::mat4 viewMatrix;

	glm::vec3 initialCameraPos;
	glm::vec3 initialCameraFront;
	glm::vec3 initialCameraUp;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	glm::vec3 cameraTarget;

	glm::vec3 cameraDirection;
	glm::vec3 cameraRight;

	glm::vec3 movementVec = glm::vec3(0.0,0.0,0.0);

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	float cameraSpeed;
	float velocity;

	float yaw;
	float pitch;



	/*void ControlCamera();*/
	void UpdateViewMatrix();
	void UpdateDirection();

	void ResetCamera();
};

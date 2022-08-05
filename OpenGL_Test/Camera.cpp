#include "Camera.h"
#include <GLFW/glfw3.h>
#include <iostream>


Camera::Camera() {

}

Camera::Camera(glm::vec3 CameraPos, glm::vec3 CameraUp, glm::vec3 target) {

	velocity = 2.5f;

	//get camera position
	cameraPos = CameraPos;
	//set initial camera target
	cameraTarget = target;
	//get inverse camera direction
	cameraDirection = target;//actually is this the inverse of the cameradirection

	cameraFront = cameraDirection;
	cameraUp = CameraUp;

	cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));
	/*cameraUp = glm::cross(cameraDirection, cameraRight);*/



	//for reset purposes
	initialCameraPos = cameraPos;
	initialCameraFront = cameraFront;
	initialCameraUp = cameraUp;


	UpdateViewMatrix();



}



void Camera::UpdateDirection() {

	//TODO DIRECTION https://learnopengl.com/Getting-started/Cameras


	//PITCH: rotation on x axis, shows how high or low the camera looks 
	//YAW: rotation on y axis, left or right rotation with camera (like turning your head)
	//ROLL: rotation on z axis, rolling around the z axis basically


	//look at the images on learnopengl.com to understand how the axis values are calculated
	//setting the direction

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));

	cameraFront = glm::normalize(direction); //its a directional vector, so we gotta normalize it


}

void Camera::UpdateViewMatrix() {

	//getting delta time
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;  

	cameraSpeed = velocity * deltaTime;

	//update camera front by getting and setting rotation
	UpdateDirection();

	//get the current camera position
	cameraPos += (movementVec.x * glm::normalize(glm::cross(cameraFront, cameraUp)) + movementVec.y * cameraUp + movementVec.z * cameraFront)* cameraSpeed;
	
	//update view matrix
	viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); //for walking


}


void Camera::ResetCamera() {
	cameraPos = initialCameraPos;
	cameraFront = initialCameraFront;
	cameraUp = initialCameraUp;

	UpdateViewMatrix();
}
#include "camera.h"

camera::~camera()
{
}

glm::mat4 camera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void camera::processKeyboard(Camera_Movement direction, GLfloat dt) {
	GLfloat velocity = movementspeed;// * dt;
	switch (direction) {
	case FORWARD:
		position += front * velocity;
		break;
	case BACKWARD:
		position -= front * velocity;
		break;
	case LEFT:
		position -= right * velocity;
		break;
	case RIGHT:
		position += right * velocity;
		break;
	default:
		break;
	}
}

void camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainpitch) {
	xoffset *= mousesensitivity;
	yoffset *= mousesensitivity;
	
	yaw += xoffset;
	pitch += yoffset;

	if (constrainpitch) {
		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		if (pitch < -89.0f) {
			pitch = -89.0f;
		}
	}

	updateCameraVectors();
}	

void camera::processMouseScroll(GLfloat yoffset) {
	if (zoom >= 1.0f && zoom <= 45.0f) {
		zoom -= yoffset;
	}
	if (zoom <= 1.0f) {
		zoom = 1.0f;
	}
	if (zoom >= 45.0f) {
		zoom = 45.0f;
	}
}

void camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldup));
	up = glm::normalize(glm::cross(right, front));
}
#include "camera.h"

camera::~camera()
{
}

glm::mat4 camera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
	//return glm::lookAt((glm::vec3(0.0f, 0.0f, 0.0f) + (20.0f * front)), glm::vec3(0.0f, 0.0f, 0.0f), up);
}

void camera::processKeyboard(Camera_Movement direction, float delta) {
	GLfloat velocity = movementspeed * delta;
	if (direction == FORWARD)
		position += front * velocity;
	if (direction == BACKWARD)
		position -= front * velocity;
	if (direction == ROTL) {
		camera_angle = glm::radians(-5.0f);
		cameraRotationMatrix = glm::rotate(cameraRotationMatrix, camera_angle, glm::vec3(0.0f, 1.0f, 0.0f));
	}		
	if (direction == ROTR) {
		camera_angle = glm::radians(5.0f);
		cameraRotationMatrix = glm::rotate(cameraRotationMatrix, camera_angle, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == RIGHT)
		position += right * velocity;
	if (direction == UP)
		position += up * velocity;
	if (direction == DOWN)
		position -= up * velocity;
	if (direction == POS1) {
		position.z = 50.0f;
		camera_angle = glm::radians(30.0f);
		cameraRotationMatrix = glm::mat4();
		cameraRotationMatrix = glm::rotate(cameraRotationMatrix, camera_angle, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (direction == POS2) {
		position.z = 50.0f;
		camera_angle = glm::radians(-30.0f);
		cameraRotationMatrix = glm::mat4();
	}
	if (direction == POS3) {
		position = glm::vec3(0.0f, 0.0f, 50.0f);
		camera_angle = glm::radians(-30.0f);
		cameraRotationMatrix = glm::mat4();
		cameraRotationMatrix = glm::rotate(cameraRotationMatrix, camera_angle, glm::vec3(0.0f, 1.0f, 0.0f));
	}

}
 
void camera::updateCameraVectors() {
	glm::vec3 tfront;
	tfront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	tfront.y = sin(glm::radians(pitch));
	tfront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	tfront = glm::normalize(tfront);
	right = glm::normalize(glm::cross(tfront, worldup));
	up = glm::normalize(glm::cross(right, tfront));
}

//position -= right * velocity; left

//position += right * velocity; right
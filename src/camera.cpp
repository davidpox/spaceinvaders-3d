#include "camera.h"

camera::~camera()
{
}

glm::mat4 camera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void camera::processKeyboard(Camera_Movement direction, GLfloat dt) {
	GLfloat velocity = movementspeed;// *dt;
	if (direction == FORWARD)
		position += front * velocity;
	if (direction == BACKWARD)
		position -= front * velocity;
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == RIGHT)
		position += right * velocity;
	if (direction == UP)
		position += up * velocity;
	if (direction == DOWN)
		position -= up * velocity;
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
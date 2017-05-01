#pragma once

#include <iostream>
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"

// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	ROTL,
	ROTR,
	POS1,
	POS2,
	POS3
};

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 50.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.0f;

class camera
{
public:
	camera(glm::vec3 tposition = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 tup = glm::vec3(0.0f, 1.0f, 0.0f),
		GLfloat tYaw = YAW,
		GLfloat tpitch = PITCH) :
		front(glm::vec3(0.0f, 0.0f, -1.0f)),
		movementspeed(SPEED),
		mousesensitivity(SENSITIVITY),
		zoom(ZOOM)
	{
		position = tposition;
		worldup = tup;
		yaw = tYaw;
		pitch = tpitch;
		updateCameraVectors();
	}
	camera(GLfloat posX,
		GLfloat posY,
		GLfloat posZ,
		GLfloat upX,
		GLfloat upY,
		GLfloat upZ,
		GLfloat tyaw,
		GLfloat tpitch) :
		movementspeed(SPEED),
		mousesensitivity(SENSITIVITY),
		zoom(ZOOM)
	{
		position = glm::vec3(posX, posY, posZ);
		worldup = glm::vec3(upX, upY, upZ);
		yaw = tyaw;
		pitch = tpitch;
		updateCameraVectors();
	}
	~camera();
	
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldup;

	GLfloat camera_angle;
	glm::mat4 cameraRotationMatrix;

	GLfloat yaw;
	GLfloat pitch;
	GLfloat movementspeed;
	GLfloat mousesensitivity;
	GLfloat zoom;

	glm::mat4 getViewMatrix();
	void processKeyboard(Camera_Movement direction, float delta);
	void updateCameraVectors();	
};


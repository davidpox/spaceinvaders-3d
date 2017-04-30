#pragma once

#include <iostream>
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"

// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "model.h"

enum BULLET_TYPE {
	PLAYER,
	ENEMY
};

class bullet : public model
{
public:
	bullet(std::string name);
	~bullet();

	void shootBullet();
	void arrangeToAlien();
	bool isActive;
	bool moved;
	void resetPositionX(glm::vec3 playerPos, SDL_Keycode dir);
	void resetPositionY();
	void resetPositionAL();

	glm::vec3 distTravelled = { 0.0f, 0.0f, 0.0f };
	glm::vec3 alTravel = { 0.0f, 0.0f, 0.0f }; 
	glm::mat4 _transRotate, _transTranslate, _transScale;

	std::string name;

	void loadModel(BULLET_TYPE btype);

	model Model;

	BULLET_TYPE type;

};


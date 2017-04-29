#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"
#include "bullet.h"

// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"

class alien : public model
{
public:
	alien();
	~alien();

	model Model;

	void moveAlien(SDL_Keycode dir);

	float timer;
	int spinofftimer;
	bool isDead;
	int currentAnimState;

};


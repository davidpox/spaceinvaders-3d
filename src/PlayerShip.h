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

class PlayerShip : public model
{
public:
	PlayerShip();
	~PlayerShip();

	model Model;

	void movePlayer(SDL_Keycode dir);
	void loadModel();

	glm::mat4 _transRotate, _transTranslate, _transScale;

};



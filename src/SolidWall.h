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

class SolidWall : public model
{
public:
	SolidWall();
	~SolidWall();
	
	model ModelL;
	model ModelR;
	model ModelT;
	model ModelB;

	void load();
};


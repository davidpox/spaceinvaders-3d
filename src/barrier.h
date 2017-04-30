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

class barrier : public model
{
public:
	barrier();
	~barrier();

	std::vector<model> barriersLeft;
	std::vector<model> barriersMiddle;
	std::vector<model> barriersRight;

	void load();
	void move();

};


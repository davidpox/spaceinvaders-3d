#include "bullet.h"



bullet::bullet(std::string name)
{
	isActive = false;
	moved = false;
	name = "blank";
	objName = name + " bullet";
	size.x = 0.2f;
	size.y = 0.2f;
	size.z = 0.5f;
}


bullet::~bullet()
{
}


void bullet::shootBullet() {
	isActive = true;
}

void bullet::resetPositionX(glm::vec3 playerPos, SDL_Keycode dir) {
	if (isActive) {
		if (dir == SDLK_a) {
			if (position[0] >= -14.0f) {
				distTravelled[0] -= 1.0f;
				moved = true;
			}
		}
		if (dir == SDLK_d) {
			if (position[0] <= 14.0f) {
				distTravelled[0] += 1.0f;
				moved = true;
			}
		}
	}
	if (moved && !isActive) {
		position[0] += distTravelled[0];
		_transTranslate = glm::translate(_transTranslate, glm::vec3(distTravelled.x, 0.0f, 0.0f));
		distTravelled[0] = 0.0f;
		moved = false;
		
	}
	if (!isActive) {
		if (position[0] >= -14.0f) {
			if (dir == SDLK_a) {
				_transTranslate = glm::translate(_transTranslate, glm::vec3(-1.0f, 0.0f, 0.0f));
				position[0] -= 1.0f;
			}
		}
		if (position[0] <= 14.0f) {
			if (dir == SDLK_d) {
				_transTranslate = glm::translate(_transTranslate, glm::vec3(1.0f, 0.0f, 0.0f));
				position[0] += 1.0f;
			}
		}
	}	
}	

void bullet::resetPositionY() {
	_transTranslate = glm::translate(_transTranslate, glm::vec3(0.0f, -distTravelled[1], 0.0f));
	position[1] -= distTravelled[1];
	distTravelled[1] = 0.0f;
}

void bullet::arrangeToAlien() {
	_transTranslate = glm::translate(_transTranslate, glm::vec3(0.0f, 0.775f, 0.0f));
	position.x += 0.775f;
}

void bullet::resetPositionAL() {
	_transTranslate = glm::translate(_transTranslate, glm::vec3(-position.x, -position.y, 0.0f));
	position.x -= position.x;
	position.y -= position.y;
} 

void bullet::loadModel(BULLET_TYPE btype) {
	if (btype == PLAYER) {
		Model = model("bin/assets/models/bullet.obj");
		position.y = -15.0f;
		_transTranslate = glm::translate(_transTranslate, glm::vec3(0.0f, -15.0f, 0.0f));
	}
	else if (btype == ENEMY) {
		Model = model("bin/assets/models/bullet.obj");
	}
}
#include "PlayerShip.h"



PlayerShip::PlayerShip()
{
	size.x = 2.0f;
	size.y = 1.3f;
	size.z = 0.2f;
	objName = "Player";
}


PlayerShip::~PlayerShip()
{
}

void PlayerShip::movePlayer(SDL_Keycode dir) {
	if (position[0] >= -14.0f) {			//&& position[0] >= -0.9f
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
		//std::cout << position.x << std::endl;
}

void PlayerShip::loadModel() {
	Model = model("bin/assets/models/playership.obj");
	_transTranslate = glm::translate(_transTranslate, glm::vec3(0.0f, -10.0f, 0.0f));
	position.y -= 10.0f;
	position.x = 0.0f;
}
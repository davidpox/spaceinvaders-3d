#include "alien.h"

alien::alien() {
	Model = model("bin/assets/models/spaceinvader.obj");
	Model2 = model("bin/assets/models/spaceinvader2.obj");


	position.x = -15.0f;
	position.y = 15.0f;

	spinofftimer = 0;
	isDead = false;
	currentAnimState = 1;

	objName = "Space Invader";

	size.x = 2.0f;
	size.y = 1.5f;
	size.z = 0.2f;


	activeModel = Model;
}

alien::~alien() {}

void alien::spinoff() {
	position.y -= 0.5f;
	_transTranslate = glm::translate(_transTranslate, glm::vec3(0.0f, -0.5f, 0.0f));
	_transRotate = glm::rotate(_transRotate, 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
	_transScale = glm::scale(_transScale, glm::vec3(-0.9f, -0.9f, -0.9f));
}


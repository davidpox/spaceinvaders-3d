#include "alien.h"

alien::alien() {
	Model = model("bin/assets/models/spaceinvader.obj");
	position.x = -10.0f;
	position.y = 10.0f;

	spinofftimer = 0;
	isDead = false;
	currentAnimState = 1;
}

alien::~alien() {

}


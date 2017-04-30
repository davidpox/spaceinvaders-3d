#include "barrier.h"

barrier::barrier()
{
	position = { 0.0f, 0.0f, 0.0f };
	size.x = 1.0f;
	size.y = 1.0f;
	size.z = 0.2f;
	barriersLeft.resize(5);
	barriersMiddle.resize(5);
	barriersRight.resize(5);
}


barrier::~barrier()
{
}

void barrier::load() {
	for (int i = 0; i < 5; i++) {
		barriersLeft[i] = model("bin/assets/models/wall.obj");
		barriersMiddle[i] = model("bin/assets/models/wall.obj");
		barriersRight[i] = model("bin/assets/models/wall.obj");
	}
	move();
}

void barrier::move() {
	std::cout << "Grouping barriers" << std::endl;
	for (int i = 0; i < 5; i++) { // Initial Group
		barriersLeft[i].position.x -= 10.0f;
		barriersLeft[i].position.y -= 5.0f;
		barriersLeft[i]._transTranslate = glm::translate(barriersLeft[i]._transTranslate, glm::vec3(-10.0f, -5.0f, 0.0f));

		barriersRight[i].position.x += 10.0f;
		barriersRight[i].position.y -= 5.0f;
		barriersRight[i]._transTranslate = glm::translate(barriersRight[i]._transTranslate, glm::vec3(10.0f, -5.0f, 0.0f));

		barriersMiddle[i].position.y -= 5.0f;
		barriersMiddle[i]._transTranslate = glm::translate(barriersMiddle[i]._transTranslate, glm::vec3(0.0f, -5.0f, 0.0f));	
	}

	std::cout << "Setting sizes..." << std::endl;
	for (int i = 0; i < 5; i++) {
		barriersLeft[i].size.x = 1.0f;
		barriersLeft[i].size.y = 1.0f;
		barriersLeft[i].size.z = 0.2f;
		barriersMiddle[i].size.x = 1.0f;
		barriersMiddle[i].size.y = 1.0f;
		barriersMiddle[i].size.z = 0.2f;
		barriersRight[i].size.x = 1.0f;
		barriersRight[i].size.y = 1.0f;
		barriersRight[i].size.z = 0.2f;
	}
	
	barriersLeft[1].position.x += 1.0f;
	barriersLeft[1]._transTranslate = glm::translate(barriersLeft[1]._transTranslate, glm::vec3(1.0f, 0.0f, 0.0f));
	barriersLeft[2].position.x -= 1.0f;
	barriersLeft[2]._transTranslate = glm::translate(barriersLeft[2]._transTranslate, glm::vec3(-1.0f, 0.0f, 0.0f));
	barriersLeft[3].position.y -= 1.0f;
	barriersLeft[3].position.x += 1.0f;
	barriersLeft[3]._transTranslate = glm::translate(barriersLeft[3]._transTranslate, glm::vec3(1.0f, -1.0f, 0.0f));
	barriersLeft[4].position.y -= 1.0f;
	barriersLeft[4].position.x -= 1.0f;
	barriersLeft[4]._transTranslate = glm::translate(barriersLeft[4]._transTranslate, glm::vec3(-1.0f, -1.0f, 0.0f));
		
	barriersMiddle[1].position.x += 1.0f;
	barriersMiddle[1]._transTranslate = glm::translate(barriersMiddle[1]._transTranslate, glm::vec3(1.0f, 0.0f, 0.0f));
	barriersMiddle[2].position.x -= 1.0f;
	barriersMiddle[2]._transTranslate = glm::translate(barriersMiddle[2]._transTranslate, glm::vec3(-1.0f, 0.0f, 0.0f));
	barriersMiddle[3].position.y -= 1.0f;
	barriersMiddle[3].position.x += 1.0f;
	barriersMiddle[3]._transTranslate = glm::translate(barriersMiddle[3]._transTranslate, glm::vec3(1.0f, -1.0f, 0.0f));
	barriersMiddle[4].position.y -= 1.0f;
	barriersMiddle[4].position.x -= 1.0f;
	barriersMiddle[4]._transTranslate = glm::translate(barriersMiddle[4]._transTranslate, glm::vec3(-1.0f, -1.0f, 0.0f));

	barriersRight[1].position.x += 1.0f;
	barriersRight[1]._transTranslate = glm::translate(barriersRight[1]._transTranslate, glm::vec3(1.0f, 0.0f, 0.0f));
	barriersRight[2].position.x -= 1.0f;
	barriersRight[2]._transTranslate = glm::translate(barriersRight[2]._transTranslate, glm::vec3(-1.0f, 0.0f, 0.0f));
	barriersRight[3].position.y -= 1.0f;
	barriersRight[3].position.x += 1.0f;
	barriersRight[3]._transTranslate = glm::translate(barriersRight[3]._transTranslate, glm::vec3(1.0f, -1.0f, 0.0f));
	barriersRight[4].position.y -= 1.0f;
	barriersRight[4].position.x -= 1.0f;
	barriersRight[4]._transTranslate = glm::translate(barriersRight[4]._transTranslate, glm::vec3(-1.0f, -1.0f, 0.0f));
}
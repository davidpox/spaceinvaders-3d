#include "SolidWall.h"



SolidWall::SolidWall()
{
	
}


SolidWall::~SolidWall()
{
}

void SolidWall::load() {
	ModelL = model("bin/assets/models/wall.obj");
	ModelR = model("bin/assets/models/wall.obj");
	ModelT = model("bin/assets/models/wall.obj");
	ModelB = model("bin/assets/models/wall.obj");

	ModelL._transTranslate = glm::translate(ModelL._transTranslate, glm::vec3(17.5f, 0.0f, 0.0f));
	ModelR._transTranslate = glm::translate(ModelR._transTranslate, glm::vec3(-17.5f, 0.0f, 0.0f));
	ModelT._transTranslate = glm::translate(ModelT._transTranslate, glm::vec3(0.0f, 17.5f, 0.0f));
	ModelB._transTranslate = glm::translate(ModelB._transTranslate, glm::vec3(0.0f, -17.5f, 0.0f));

	ModelL._transScale = glm::scale(ModelL._transScale, glm::vec3(2.0f, 35.0f, 10.0f));
	ModelR._transScale = glm::scale(ModelR._transScale, glm::vec3(2.0f, 35.0f, 10.0f));
	ModelT._transScale = glm::scale(ModelT._transScale, glm::vec3(2.0f, 37.0f, 10.0f));
	ModelB._transScale = glm::scale(ModelB._transScale, glm::vec3(2.0f, 37.0f, 10.0f));


	ModelT._transRotate = glm::rotate(ModelT._transRotate, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 90.0f));
	ModelB._transRotate = glm::rotate(ModelB._transRotate, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 90.0f));
}
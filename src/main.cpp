// Basic Libraries
#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
// GLEW - must include before SDL
#include <GL/glew.h>
// SDL
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
// Classes
#include "PlayerShip.h"
#include "bullet.h"
#include "alien.h"
#include "gamestate.h"
#include "barrier.h"
#include "Background.h"
#include "SolidWall.h"
#include "camera.h"
#include "shotHandler.h" 

#include "model.h"

//GLM
#define GLM_FORCE_RADIANS // force glm to use radians
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SDL_Window *win = nullptr;
SDL_GLContext mainContext;
PlayerShip player;
bullet playerBullet("player");
bullet alienBullet("alien");
barrier barriers;
shotHandler sh;
gamestate gs;
camera cam(glm::vec3(0.0f, 0.0f, 50.0f));
SolidWall sw;
Background bg;
std::vector<alien> aliens;
glm::vec3 lightPos(0.0f, 0.0f, 40.0f);
GLint program;
std::vector<PlayerShip> lives;

GLuint test_vao;

bool isGameRunning;

void SetOpenGLAttr() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

int init() {
	int screenWidth, screenHeight;
	int winWidth, winHeight;
	SDL_DisplayMode disp;

	// SDL initialise
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n",
			glGetError());
		return 1;
	}
	SDL_Log("SDL initialised OK!\n");

	if (SDL_GetCurrentDisplayMode(0, &disp) == 0) {
		screenWidth = disp.w;
		screenHeight = disp.h;
	}
	else {
		std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
		return 1;
	}


	// Window Creation
	win = SDL_CreateWindow("David Puetter; CGP2012M; PUE15564059", screenWidth/4, screenHeight/4, screenWidth/2, screenHeight/2, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (win == nullptr) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"SDL_CreateWindow init error: %s\n", glGetError());
		return 1;
	}
	SDL_GetWindowSize(win, &winWidth, &winHeight);


	// SDL_Image initialise
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_image init error: %s\n",
			IMG_GetError());
		return 1;
	}
	SDL_Log("SDL_image initialised OK!\n");

	// SDL_ttf initialise
	if (TTF_Init() == -1) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_ttf init error: %s\n",
			glGetError());
		return 1;
	}
	SDL_Log("SDL_ttf initialised OK!\n");


	/* CONTEXT AND GLEW CREATION */

	SetOpenGLAttr();

	mainContext = SDL_GL_CreateContext(win);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, screenWidth / 2, screenHeight / 2);
	gs.windowWidth = screenWidth / 2;
	gs.windowHeight = screenHeight / 2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	return 0;
}

void resizeWindow() {
	int w, h;
	SDL_GetWindowSize(win, &w, &h);

	glViewport(0, 0, w, h);

	gs.windowHeight = h;
	gs.windowWidth = w;
}

void getInput(float dt) {
	SDL_Event ev;
	while (SDL_PollEvent(&ev) != 0) {
		if (ev.type == SDL_KEYDOWN) {
			switch (ev.key.keysym.sym)
			{
			case SDLK_a:
			case SDLK_d:
				player.movePlayer(ev.key.keysym.sym);
				playerBullet.resetPositionX(player.position, ev.key.keysym.sym);
				break;
			case SDLK_SPACE:
				playerBullet.shootBullet();
				break;
			case SDLK_ESCAPE:
				SDL_Quit();
				gs.isGameRunning = false;
				break;
			case SDLK_k:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_BLEND);
				break;
			case SDLK_l:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_BLEND);
				break;
			case SDLK_F10:
				SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
				break;
			case SDLK_F11:
				SDL_SetWindowFullscreen(win, 0);
				break;
			default:
				break;
			}
			if (ev.key.keysym.sym == SDLK_KP_8)
				cam.processKeyboard(FORWARD, dt);
			if (ev.key.keysym.sym == SDLK_KP_5)
				cam.processKeyboard(BACKWARD, dt);
			if (ev.key.keysym.sym == SDLK_KP_4)
				cam.processKeyboard(LEFT, dt);
			if (ev.key.keysym.sym == SDLK_KP_6)
				cam.processKeyboard(RIGHT, dt);
			if (ev.key.keysym.sym == SDLK_KP_7)
				cam.processKeyboard(DOWN, dt);
			if (ev.key.keysym.sym == SDLK_KP_9)
				cam.processKeyboard(UP, dt);
			if (ev.key.keysym.sym == SDLK_KP_1)
				cam.processKeyboard(ROTL, dt);
			if (ev.key.keysym.sym == SDLK_KP_3)
				cam.processKeyboard(ROTR, dt);
			if (ev.key.keysym.sym == SDLK_1)
				cam.processKeyboard(POS1, dt);
			if (ev.key.keysym.sym == SDLK_2)
				cam.processKeyboard(POS2, dt);
			if (ev.key.keysym.sym == SDLK_3)
				cam.processKeyboard(POS3, dt);
		}
		else if (ev.type == SDL_WINDOWEVENT) {
			if (ev.window.event == SDL_WINDOWEVENT_RESIZED) {
				resizeWindow();
			}
		}
		else if (ev.type == SDL_QUIT) {
			gs.isGameRunning = false;
		}
	}
}

void moveAliens() {
	if (!gs.isMovingLeft) {
		if (gs.alienMoveCounter < 2) {
			for (int i = 0; i < aliens.size(); i++) {
				if (!aliens[i].isDead) {
					aliens[i]._transTranslate = glm::translate(aliens[i]._transTranslate, glm::vec3(2.5f, 0.0f, 0.0f));
					aliens[i].position.x += 2.5f;
				}
			}
			gs.alienMoveCounter++;
		}
		else if (gs.alienMoveCounter == 2) {
			for (int i = 0; i < aliens.size(); i++) {
				if (!aliens[i].isDead) {
					aliens[i]._transTranslate = glm::translate(aliens[i]._transTranslate, glm::vec3(0.0f, -2.5f, 0.0f));
					aliens[i].position.y -= 2.5f;
				}
			}
			gs.alienMoveSpeed += 0.15f;
			gs.alienMoveCounter = 0;
			gs.isMovingLeft = true;
		}
	}
	else if (gs.isMovingLeft) {
		if (gs.alienMoveCounter < 2) {
			for (int i = 0; i < aliens.size(); i++) {
				if (!aliens[i].isDead) {
					aliens[i]._transTranslate = glm::translate(aliens[i]._transTranslate, glm::vec3(-2.5f, 0.0f, 0.0f));
					aliens[i].position.x -= 2.5f;
				}
			}
			gs.alienMoveCounter++;
		}
		else if (gs.alienMoveCounter == 2) {
			for (int i = 0; i < aliens.size(); i++) {
				if (!aliens[i].isDead) {
					aliens[i]._transTranslate = glm::translate(aliens[i]._transTranslate, glm::vec3(0.0f, -2.5f, 0.0f));
					aliens[i].position.y -= 2.5f;
				}
			}
			gs.alienMoveSpeed += 0.15f; // Increase speed by .15f every time Space Invaders shift downwards
			gs.alienMoveCounter = 0;
			gs.isMovingLeft = false;
		}
	}
}

void alienShoot() {
	if (!alienBullet.isActive) {
		int it = sh.getAlien(aliens);
		if (it != -1) {
			alien alien_t = aliens[it];
			alienBullet.position.x = alien_t.position.x - 0.025f;
			alienBullet.position.y = alien_t.position.y;
			alienBullet._transTranslate = glm::translate(alienBullet._transTranslate, glm::vec3(alien_t.position.x - 0.025f, alien_t.position.y, 0.0f));
			alienBullet.isActive = true;
		}
	}
}

void update() {
	if (playerBullet.isActive) {
		playerBullet._transTranslate = glm::translate(playerBullet._transTranslate, glm::vec3(0.0f, 1.0f, 0.0f));
		playerBullet.position.y += 1.0f;
		playerBullet.distTravelled.y += 1.0f;
	}

	if (playerBullet.position[1] >= 15.0f) {
		playerBullet.isActive = false;
		playerBullet.resetPositionY();
		playerBullet.resetPositionX(player.position, SDLK_UP);
	}

	if (alienBullet.isActive) {
		alienBullet._transTranslate = glm::translate(alienBullet._transTranslate, glm::vec3(0.0f, -0.8f, 0.0f));
		alienBullet.position.y -= 0.8f;
		alienBullet.distTravelled.y -= 0.8f;
	}
	if (alienBullet.position.y <= -17.5f) {
		alienBullet.isActive = false;
		alienBullet.resetPositionAL();
	}

	if (gs.alienMoveTimer == (int)(120 / gs.alienMoveSpeed)) {
		moveAliens();
		for (int i = 0; i < aliens.size(); i++) {
			if (aliens[i].currentAnimState == 2) {
				aliens[i].activeModel = aliens[i].Model;
				aliens[i].currentAnimState = 1;
			}
			else if (aliens[i].currentAnimState == 1) {
				aliens[i].activeModel = aliens[i].Model2;
				aliens[i].currentAnimState = 2;
			}
		}
		gs.alienMoveTimer = 0;
	}
	else {
		gs.alienMoveTimer++;
	}

	if (aliens.size() == 0) {
		gs.gameover = true;
	}

	if (gs.alienShootTimer == gs.rndShot * 60) {
		alienShoot();
		gs.alienShootTimer = 0;
		gs.rndShot = glm::linearRand(1, 3);

	}
	gs.alienShootTimer++;
	for (int i = 0; i < aliens.size(); i++) {
		if (aliens[i].isDead) {
			aliens[i].spinoff();
			if (aliens[i].position.y < -7.5f) {
				aliens.erase(aliens.begin() + i);
			}
		}
	}
}

void render() {

	glm::vec4 backgroundColour = glm::vec4(0.1, 0.1, 0.1, 1.0);		// R, G, B, A
	glClearColor(backgroundColour[0], backgroundColour[1], backgroundColour[2], backgroundColour[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//renderOrthographic();

	if (!gs.gameover) {

		// CUBEMAP
		glDepthFunc(GL_LEQUAL);
		glUseProgram(bg.program);
		glm::mat4 projection = glm::perspective(glm::radians(cam.zoom), (float)gs.windowWidth / (float)gs.windowHeight, 0.1f, 1000.0f);
		glm::mat4 view = glm::mat4(glm::mat3(cam.getViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(bg.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(bg.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		bg.Draw();
		glDepthFunc(GL_LESS);

		// LIGHTNING
		glUseProgram(program);
		lightPos = cam.position;
		glUniform3fv(glGetUniformLocation(program, "light.position"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, &cam.position[0]);

		glUniform3f(glGetUniformLocation(program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(program, "light.specular"), 1.0f, 1.0f, 1.0f);

		glUniform3f(glGetUniformLocation(program, "material.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(program, "light.shininess"), 32.0f);

		glUniform1f(glGetUniformLocation(program, "light.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(program, "light.linear"), 0.007f);
		glUniform1f(glGetUniformLocation(program, "light.quadratic"), 0.0002f);

		// MODELS
		//projection = glm::perspective(glm::radians(cam.zoom), (float)gs.windowWidth / (float)gs.windowHeight, 0.1f, 1000.0f);
		view = cam.getViewMatrix();
		glm::mat4 transModel;
		GLint modelLoc = glGetUniformLocation(program, "model");
		GLint viewLoc = glGetUniformLocation(program, "view");
		GLint projLoc = glGetUniformLocation(program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view * cam.cameraRotationMatrix));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));// *cam.cameraRotationMatrix));



		for (int i = 0; i < aliens.size(); i++) {
			transModel = aliens[i]._transTranslate * aliens[i]._transRotate * aliens[i]._transScale;
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transModel));
			aliens[i].activeModel.Draw(program);
		}

		transModel = player._transTranslate * player._transRotate * player._transScale;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transModel));
		player.Model.Draw(program);


		if (playerBullet.isActive) {
			transModel = playerBullet._transTranslate * playerBullet._transRotate * playerBullet._transScale;
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transModel));
			playerBullet.Model.Draw(program);
		}

		if (alienBullet.isActive) {
			transModel = alienBullet._transTranslate * alienBullet._transRotate * alienBullet._transScale;
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transModel));
			alienBullet.Model.Draw(program);
		}


		for (int i = 0; i < barriers.barriersLeft.size(); i++) {
			transModel = barriers.barriersLeft[i]._transTranslate *  barriers.barriersLeft[i]._transRotate *  barriers.barriersLeft[i]._transScale;
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transModel));
			barriers.barriersLeft[i].Draw(program);
		}
		for (int i = 0; i < barriers.barriersMiddle.size(); i++) {
			transModel = barriers.barriersMiddle[i]._transTranslate *  barriers.barriersMiddle[i]._transRotate *  barriers.barriersMiddle[i]._transScale;
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transModel));
			barriers.barriersMiddle[i].Draw(program);
		}
		for (int i = 0; i < barriers.barriersRight.size(); i++) {
			transModel = barriers.barriersRight[i]._transTranslate *  barriers.barriersRight[i]._transRotate *  barriers.barriersRight[i]._transScale;
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transModel));
			barriers.barriersRight[i].Draw(program);
		}

		transModel = sw.ModelL._transTranslate * sw.ModelL._transRotate * sw.ModelL._transScale;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transModel));
		sw.ModelL.Draw(program);

		transModel = sw.ModelR._transTranslate * sw.ModelR._transRotate * sw.ModelR._transScale;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transModel));
		sw.ModelR.Draw(program);

		transModel = sw.ModelT._transTranslate * sw.ModelT._transRotate * sw.ModelT._transScale;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transModel));
		sw.ModelT.Draw(program);

		transModel = sw.ModelB._transTranslate * sw.ModelB._transRotate * sw.ModelB._transScale;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transModel));
		sw.ModelB.Draw(program);

		if (lives.size() >= 1) {
			if (lives.size() >= 2) {
				if (lives.size() == 3) {
					transModel = lives[2]._transTranslate * lives[2]._transRotate * lives[2]._transScale;
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transModel));
					lives[2].Model.Draw(program);
				}
				transModel = lives[1]._transTranslate * lives[1]._transRotate * lives[1]._transScale;
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transModel));
				lives[1].Model.Draw(program);
			}
			transModel = lives[0]._transTranslate * lives[0]._transRotate * lives[0]._transScale;
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transModel));
			lives[0].Model.Draw(program);
		}
	}

	SDL_GL_SwapWindow(win);
}

template <typename A, typename B>
bool checkCollisions(A a, B b) {
	float left_src, left_dst;
	float right_src, right_dst;
	float top_src, top_dst;
	float bottom_src, bottom_dst;


	left_src = a.position.x;
	right_src = a.position.x + a.size.x;
	top_src = a.position.y;
	bottom_src = a.position.y + a.size.y;

	left_dst = b.position.x;
	right_dst = b.position.x + b.size.x;
	top_dst = b.position.y;
	bottom_dst = b.position.y + b.size.y;

	if (bottom_src <= top_dst) {
		return false;
	}
	if (top_src >= bottom_dst) {
		return false;
	}
	if (right_src <= left_dst) {
		return false;
	}
	if (left_src >= right_dst) {
		return false;
	}
	return true;
}



void setupAliens() {
	float xOffset = 0.0f;	// used to initialise positions
	float yOffset = 0.0f;
	int count = 0;

	/* Fill alien vector with aliens */
	for (int i = 0; i < 55; i++) {
		aliens.push_back(alien());
	}
	std::cout << aliens.size() << " aliens created" << std::endl;
	for (int i = 0; i < aliens.size(); i++) {
		++count;

		if (i % 11 == 0 && i != 0) {
			yOffset -= 2.0f; 
			xOffset = 0.0f;
		}

		aliens[i].position.x += xOffset;
		aliens[i].position.y += yOffset;
		aliens[i]._transTranslate = glm::translate(aliens[i]._transTranslate, glm::vec3(aliens[i].position.x, aliens[i].position.y, 0.0f));

		xOffset += 2.5f;
	}
	std::cout << "All aliens arranged successfully" << std::endl;
}

void setupEntities() {
	/* Creating Objects */
	setupAliens();
	player.loadModel();
	program = player.createShaderProgram();
	alienBullet.loadModel(ENEMY);
	playerBullet.loadModel(PLAYER);

	//Init Alien bullets once

	alienShoot(); 

	// BARRIERS
	barriers.load();

	//WALLS
	sw.load();

	//CUBEMAP
	bg.makeCubeMap();
	bg.createShaderProgram();
	bg.createCube();
	// LIGHT
	std::cout << "LIGHT SETUP" << std::endl;
	glUniform1i(glGetUniformLocation(program, "material.diffuse"), 0);


	lives.resize(3);
	//LIVES
	lives[0].loadModel();
	lives[0]._transTranslate = glm::translate(lives[0]._transTranslate, glm::vec3(-2.5f, -5.0f, 0.0f));
	lives[1].loadModel();
	lives[1]._transTranslate = glm::translate(lives[1]._transTranslate, glm::vec3(0.0f, -5.0f, 0.0f));
	lives[2].loadModel();
	lives[2]._transTranslate = glm::translate(lives[2]._transTranslate, glm::vec3(2.5f, -5.0f, 0.0f));
}


int main(int argc, char *argv[]) {

	gs.isGameRunning = true;
	float frametime = 0.0f;
	int prevtime = 0;
	int currenttime = 0;
	float deltatime = 0.0f;

	if (init() == 1) {
		std::cout << "Could not init" << std::endl;
		system("PAUSE");
		return 1;
	}
	std::cout << "OPENGL Version: " << glGetString(GL_VERSION) << std::endl;
	
	setupEntities();
	gs.rndShot = glm::linearRand(1, 3);
	gs.gameover = false;

	while (gs.isGameRunning) {
		prevtime = currenttime;
		currenttime = SDL_GetTicks();
		deltatime = (currenttime - prevtime) / 1000.0f;
		frametime += deltatime;

		getInput(deltatime);

		if (frametime >= 0.01667f) {

			update();

			frametime = 0.0f;
		}

		if (playerBullet.isActive) {
			for (int i = 0; i < aliens.size(); i++) {
				if (!aliens[i].isDead) {
					if (checkCollisions(aliens[i], playerBullet)) {
						aliens[i].isDead = true;
						playerBullet.isActive = false;
						playerBullet.resetPositionY();
						playerBullet.resetPositionX(player.position, SDLK_UP);
					}
				}
			}
		}

		for (int i = 0; i < barriers.barriersLeft.size(); i++) {
			if (checkCollisions(playerBullet, barriers.barriersLeft[i])) {
				barriers.barriersLeft.erase(barriers.barriersLeft.begin() + i);
				playerBullet.isActive = false;
				playerBullet.resetPositionY();
				playerBullet.resetPositionX(player.position, SDLK_UP);
			}
		}
		for (int i = 0; i < barriers.barriersMiddle.size(); i++) {
			if(checkCollisions(playerBullet, barriers.barriersMiddle[i])) {
				barriers.barriersMiddle.erase(barriers.barriersMiddle.begin() + i);
				playerBullet.isActive = false;
				playerBullet.resetPositionY();
				playerBullet.resetPositionX(player.position, SDLK_UP);
			}
		}
		for (int i = 0; i < barriers.barriersRight.size(); i++) {
			if (checkCollisions(playerBullet, barriers.barriersRight[i])) {
				barriers.barriersRight.erase(barriers.barriersRight.begin() + i);
				playerBullet.isActive = false;
				playerBullet.resetPositionY();
				playerBullet.resetPositionX(player.position, SDLK_UP);
			}
		}
		for (int i = 0; i < barriers.barriersLeft.size(); i++) {
			if (checkCollisions(alienBullet, barriers.barriersLeft[i])) {
				barriers.barriersLeft.erase(barriers.barriersLeft.begin() + i);
				alienBullet.isActive = false;
				alienBullet.resetPositionAL();
			}
		}
		for (int i = 0; i < barriers.barriersMiddle.size(); i++) {
			if(checkCollisions(alienBullet, barriers.barriersMiddle[i])) {
				barriers.barriersMiddle.erase(barriers.barriersMiddle.begin() + i);
				alienBullet.isActive = false;
				alienBullet.resetPositionAL();
			}
		}
		for (int i = 0; i < barriers.barriersRight.size(); i++) {
			if (checkCollisions(alienBullet, barriers.barriersRight[i])) {
				barriers.barriersRight.erase(barriers.barriersRight.begin() + i);
				alienBullet.isActive = false;
				alienBullet.resetPositionAL();
			}
		}

		if (checkCollisions(alienBullet, player)) {
			if (lives.size() > 1) {
				lives.pop_back();
			}
			else if (lives.size() == 0) {
				gs.gameover = true;
			}
		}
		render();
	}

	SDL_Log("Finished. Cleaning up and closing down\n");

	SDL_Quit();
	return 0;
}
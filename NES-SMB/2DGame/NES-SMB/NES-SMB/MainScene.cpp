#include <iostream>
#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "MainScene.h"
#include "Game.h"
#include <GL/glut.h>

#define SCREEN_X 0
#define SCREEN_Y 0
#define MAX_ARROW 146.f*2.f
#define MIN_ARROW  ((146.f*2.f) + (32.f * 2.f))

enum ArrowAnims
{
	ARROW
};

MainScene::MainScene()
{

}


MainScene::~MainScene()
{

}

void MainScene::reset()
{

}

void MainScene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/Screens/main_screen.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	
	spritesheetArrow.loadFromFile("images/arrow4.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteArrow = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f, 1.f), &spritesheetArrow, &texProgram);
	spriteArrow->setNumberAnimations(1);
	spriteArrow->addKeyframe(ARROW, glm::vec2(0.f, 0.f));

	posArrow = glm::vec2(75.f*2.f, 146.f*2.f);
	spriteArrow->setPosition(glm::vec2(posArrow.x, posArrow.y));
	counter = 0;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	centerCam = 256.f;
}

void MainScene::spriteArrowMovement() {
	if (counter > 0) {
		counter -= 1;
	}
	else {
		if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			counter = 10;
			posArrow.y -= 32.f;
			if (posArrow.y < MAX_ARROW) {
				posArrow.y = MIN_ARROW;
			}
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			counter = 10;
			posArrow.y += 32.f;
			if (posArrow.y > MIN_ARROW) {
				posArrow.y = MAX_ARROW;
			}
		}

		spriteArrow->setPosition(glm::vec2(posArrow.x, posArrow.y));
	}
}

int MainScene::update(int deltaTime)
{
	if (Game::instance().getKey('d'))
		return 0;
	else {
		spriteArrowMovement();
	}
	if (Game::instance().getKey(13)) {
		if (posArrow.y == 146.f*2.f)
			return 0;
		else if (posArrow.y == 146.f*2.f + 32.f) {
			//return Instruction scene int;
		}
		else if (posArrow.y == 146 * 2 + 64.f) {
			//return Credit scene int;
		}
	}
	return 1;
}

void MainScene::render()
{
	glm::mat4 modelview;
	texProgram.use();

	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	
	map->render();
	spriteArrow->render();
}

void MainScene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}
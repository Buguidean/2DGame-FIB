#include "CreditScene.h"
#include "Game.h"
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define SCREEN_X 0
#define SCREEN_Y 0

CreditScene::CreditScene()
{
}


CreditScene::~CreditScene()
{
}


void CreditScene::init()
{
	initShaders();
	tilesheetMap.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	map = TileMap::createTileMap("levels/Screens/credits.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, tilesheetMap);
	
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	//currentTime = 0.0f;
	//centerCam = 256.f;
}

int CreditScene::update(int deltaTime) {
	if (Game::instance().getKey('b')) {
		return 1;
	}
	return 2;
}

void CreditScene::render()
{
	glm::mat4 modelview;
	texProgram.use();

	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();
}

void CreditScene::initShaders()
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
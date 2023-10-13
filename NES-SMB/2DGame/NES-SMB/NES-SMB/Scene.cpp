#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 3
#define INIT_PLAYER_Y_TILES 12

#define INIT_ENEMY_X_TILES 5
#define INIT_ENEMY_Y_TILES 12


Scene::Scene()
{
	map = NULL;
	player = NULL;
	enemy = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if(enemy != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	enemy = new Enemy();
	enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy->setPosition(glm::vec2(INIT_ENEMY_X_TILES * map->getTileSize(), INIT_ENEMY_Y_TILES * map->getTileSize()));
	enemy->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	centerCam = 256.f;
}


void Scene::update(int deltaTime)
{
	currentTime += deltaTime;

	if (enemy != NULL) {
		enemy->update(deltaTime);
	}
	if (enemy != NULL && enemy->killed())
		enemy = NULL;

	player->update(deltaTime);
	if (enemy != NULL && enemy->playerKilled()) {
		player->killAnimation();
		// player = NULL;
	}

	else {
		glm::ivec2 pos = player->getPosition();
		if (enemy != NULL) {
			enemy->obtainPosPlayer(pos);
		}

		float v = player->getVelocity();
		int icenter = int(centerCam);
		int diff = icenter - pos.x;

		if (diff >= 256) {
			player->margin(true, icenter);
		}

		else if (player->moving()) {
			player->margin(false, icenter);
			if (diff <= 16) {
				projection = glm::translate(projection, glm::vec3(-v, 0.f, 0.f));
				centerCam += v;
			}

			else if (diff <= 105) {
				projection = glm::translate(projection, glm::vec3(-(v/3.f), 0.f, 0.f));
				centerCam += (v/3.f);
			}
		}
	}
}

void Scene::render()
{
	glm::mat4 modelview;
	texProgram.use();

	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	// if (!enemy->playerKilled())
	player->render();
	if (enemy!=NULL) 
		enemy->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



#include "PlayScene.h"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 3
#define INIT_PLAYER_Y_TILES 12

#define INIT_ENEMY_X_TILES 37
#define INIT_ENEMY_Y_TILES 12

PlayScene::PlayScene()
{
	map = NULL;
	back = NULL;
	timer.resize(3, nullptr);
	player = NULL;
	goomba = NULL;
	koopa = NULL;
	flag = NULL;
	engine = NULL;
}

PlayScene::~PlayScene()
{
	if (map != NULL)
		delete map;
	if (back != NULL)
		delete back;
	if (player != NULL)
		delete player;
	if (goomba != NULL)
		delete goomba;
	if (koopa != NULL)
		delete koopa;
	if (flag != NULL)
		delete flag;
	if (!std::all_of(timer.begin(), timer.end(), [](Text* ptr) {return ptr == nullptr;})){
		for (Text* ptr : timer) {
			delete ptr;
		}
	}
	if (engine != NULL)
		delete engine;
}

void PlayScene::init()
{
	ticks = 400.0f;
	initShaders();
	engine = irrklang::createIrrKlangDevice();
	map = TileMap::createTileMap("levels/1-1/1-1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	back = TileMap::createTileMap("levels/1-1/1-1b.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	for (auto & digit : timer) {
		digit = new Text();
	}

	timer[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	timer[0]->setPosition(glm::vec2(25 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	timer[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	timer[1]->setPosition(glm::vec2(26 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	timer[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	timer[2]->setPosition(glm::vec2(27 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	goomba = new Goomba();
	goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	goomba->setPosition(glm::vec2((INIT_ENEMY_X_TILES) * map->getTileSize(), INIT_ENEMY_Y_TILES * map->getTileSize()));
	goomba->setTileMap(map);

	koopa = new Koopa();
	koopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	koopa->setPosition(glm::vec2((INIT_ENEMY_X_TILES-10) * map->getTileSize(), INIT_ENEMY_Y_TILES * map->getTileSize()));
	koopa->setTileMap(map);

	flag = new Flag();
	flag->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	flag->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	centerCam = 256.f;
}

int PlayScene::update(int deltaTime)
{
	if (ticks > 0.f)
		ticks -= deltaTime/1000.f;

	if (ticks < 0.f || ticks < 0.005f)
		ticks = 0.f;

	// DIGITS OF THE TIMER
	int cen = (int(ticks) / 100) % 10;
	int des = (int(ticks) / 10) % 10;
	int uni = int(ticks) % 10;
	
	// TIMER ACTUALIZATION
	timer[0]->setNumber(cen);
	timer[1]->setNumber(des);
	timer[2]->setNumber(uni);

	if (!(engine->isCurrentlyPlaying("sounds/lvlMusic.ogg"))) {
		engine->play2D("sounds/lvlMusic.ogg", true, false, true)->setVolume(0.2f);
	}

	if (Game::instance().getKey('c')) {
		engine->stopAllSounds();
		return 1;
	}

	currentTime += deltaTime;

	if (goomba != NULL) {
		goomba->update(deltaTime);
	}
	if (koopa != NULL) {
		koopa->update(deltaTime);
	}

	if (goomba != NULL && goomba->killed())
		goomba = NULL;

	if (koopa != NULL && koopa->killed())
		koopa = NULL;

	player->update(deltaTime);
	if (goomba != NULL && goomba->playerKilled()) {
		player->killAnimation();
		// player = NULL;
	}
	else if (goomba != NULL && goomba->isHit()) {
		player->hit();
		goomba->setHit();
	}
	else if (koopa != NULL && koopa->playerKilled()) {
		player->killAnimation();
		// player = NULL;
	}
	else if (koopa != NULL && koopa->isHit()) {
		player->hit();
		koopa->setHit();
	}
	else {
		glm::ivec2 pos = player->getPosition();
		if (goomba != NULL) {
			goomba->obtainPosPlayer(pos);
			goomba->setMarioSpriteSize(player->getMarioSpriteSize());
			goomba->setStarMario(player->isStarMario());
		}
		if (koopa != NULL) {
			koopa->obtainPosPlayer(pos);
			koopa->setMarioSpriteSize(player->getMarioSpriteSize());
			koopa->setStarMario(player->isStarMario());
		}

		float v = player->getVelocity();
		int icenter = int(centerCam);
		int diff = icenter - pos.x;

		if (diff >= 256) {
			player->margin(true, icenter);
		}

		else if (player->moving() && (centerCam + 256.f <= 6751.f)) {
			player->margin(false, icenter);
			if (diff <= 20) {
				projection = glm::translate(projection, glm::vec3(-v, 0.f, 0.f));
				for (auto & digit : timer) {
					digit->setPosition(glm::fvec2(digit->getPosition().x + v, digit->getPosition().y));
				}
				centerCam += v;
			}

			else if (diff <= 130) {
				projection = glm::translate(projection, glm::vec3(-(v / 2.f), 0.f, 0.f));
				for (auto & digit : timer) {
					digit->setPosition(glm::fvec2(digit->getPosition().x + (v/2), digit->getPosition().y));
				}
				centerCam += (v / 2.f);
			}
		}
	}



	flag->update(deltaTime, player->getPosition());
	if (flag->getIsMario())
		player->setInFlag();


	return 0;
}

void PlayScene::render()
{
	glm::mat4 modelview;
	texProgram.use();

	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	back->render();
	map->render();
	// if (!enemy->playerKilled())
	flag->render();
	player->render();

	timer[0]->render();
	timer[1]->render();
	timer[2]->render();

	if (goomba != NULL)
		goomba->render();

	if (koopa != NULL)
		koopa->render();
}

void PlayScene::initShaders()
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
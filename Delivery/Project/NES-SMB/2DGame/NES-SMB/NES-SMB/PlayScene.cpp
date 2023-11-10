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
	map1 = NULL;
	map2 = NULL;
	transition = NULL;
	game_over = NULL;
	time_up = NULL;
	back = NULL;
	back1 = NULL;
	back2 = NULL;
	sprites = NULL;
	sprites1 = NULL;
	sprites2 = NULL;
	powerups = NULL;
	powerups1 = NULL;
	powerups2 = NULL;
	timer.resize(3, nullptr);
	point_counter.resize(6, nullptr);
	coin_counter.resize(2, nullptr);
	level = NULL;
	player = NULL;
	flag = NULL;
	engine = NULL;
}

PlayScene::~PlayScene()
{
	if (map1 != NULL)
		delete map1;
	if (map2 != NULL)
		delete map2;
	if (time_up != NULL)
		delete time_up;
	if (transition != NULL)
		delete transition;
	if (game_over != NULL)
		delete game_over;
	if (back != NULL)
		delete back;
	if (sprites != NULL)
		delete sprites;
	if (player != NULL)
		delete player;
	if (flag != NULL)
		delete flag;
	if (!std::all_of(timer.begin(), timer.end(), [](Text* ptr) {return ptr == nullptr;})){
		for (Text* ptr : timer) {
			delete ptr;
		}
	}
	if (!std::all_of(timer.begin(), timer.end(), [](Text* ptr) {return ptr == nullptr; })) {
		for (Text* ptr : point_counter) {
			delete ptr;
		}
	}
	if (engine != NULL)
		delete engine;
}

void PlayScene::clean_up()
{
	blocks_in_motion.clear();
	distances.clear();

	for (auto & points_s : points_sprites) {
		if (points_s != NULL) {
			delete points_s;
			points_s = NULL;
		}
	}
	points_sprites.clear();
	for (auto & particle_s : particles) {
		if (particle_s != NULL) {
			delete particle_s;
			particle_s = NULL;
		}
	}
	particles.clear();
	for (auto & block_s : blocks) {
		if (block_s != NULL) {
			delete block_s;
			block_s = NULL;
		}
	}
	blocks.clear();
	for (auto & powerUP_s : power_sprites) {
		if (powerUP_s != NULL) {
			delete powerUP_s;
			powerUP_s = NULL;
		}
	}
	power_sprites.clear();
	for (auto & koopa_s : koopas) {
		if (koopa_s != NULL) {
			delete koopa_s;
			koopa_s = NULL;
		}
	}
	for (auto & goomba_s : goombas) {
		if (goomba_s != NULL) {
			delete goomba_s;
			goomba_s = NULL;
		}
	}
	goombas.clear();

	delete map1;
	map1 = NULL;
	map1 = TileMap::createTileMap("levels/1-1/1-1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, tilesheetMap);

	delete map2;
	map2 = NULL;
	map2 = TileMap::createTileMap("levels/1-2/1-2.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, tilesheetMap);
}

void PlayScene::reset()
{
	finish = false;
	clean_up();

	if (curr_level == 1) {
		map = map1;
		back = back1;
		sprites = sprites1;
		powerups = powerups1;
	}
	else {
		map = map2;
		back = back2;
		sprites = sprites2;
		powerups = powerups2;
	}

	/*
	if (game_over_ret == 1) {
		transition_time = 6.f;
	}
	*/
	game_over_ret = 0;
	index = 0;
	index_pk = 0;
	active = false;
	ticks = 400.0f;
	points = 0.0f;
	coins = 0;
	star_timer = 0.f;
	points_timer = 0.f;
	//transition_time = 0.f;
	inv_timer = 0.f;

	timer[0]->setPosition(glm::vec2(26 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	timer[1]->setPosition(glm::vec2(27 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	timer[2]->setPosition(glm::vec2(28 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));

	point_counter[0]->setPosition(glm::vec2(3 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[1]->setPosition(glm::vec2(4 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[2]->setPosition(glm::vec2(5 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[3]->setPosition(glm::vec2(6 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[4]->setPosition(glm::vec2(7 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[5]->setPosition(glm::vec2(8 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));

	coin_counter[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	coin_counter[0]->setPosition(glm::vec2(13 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	coin_counter[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	coin_counter[1]->setPosition(glm::vec2(14 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));

	level = new Text();
	level->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	level->setPosition(glm::vec2(21 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	level->setNumber(1);


	level_tran->setNumber(curr_level);
	world_tran->setNumber(1);
	lives_sp->setNumber(lives);


	staticSprite = new StaticIface();
	staticSprite->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetStatic);
	staticSprite->setPosition(glm::vec2(0.f, map->getTileSize() / 2));

	//level->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	level->setPosition(glm::vec2(21 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	level->setNumber(curr_level);

	world->setPosition(glm::vec2(19 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	world->setNumber(1);

	coinSprite->setPosition(glm::vec2(11 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));

	staticSprite->setPosition(glm::vec2(0.f, map->getTileSize() / 2));
	//initShaders();
	//if (engine == NULL)
	//engine = irrklang::createIrrKlangDevice();
	//engine->play2D("sounds/lvlMusic.wav", true);

	if (player == NULL) {
		player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetM, spritesheetSM, spritesheetSuperStM, spritesheetSmallStM, spritesheetChange);
		player->setTileMap(map);
	}
	else {
		player->reset();
	}
	
	player->setTileMap(map);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));

	/*
	for (unsigned int i = 0 ; i < goombas.size() ; ++i) {
		if (goombas[i] == NULL) {
			goombas[i] = new Goomba();
			goombas[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			goombas[i]->setTileMap(map);
		}
		else {
			goombas[i]->reset();
		}

		goombas[i]->setPosition(pos_goombas[i]);
	}

	for (unsigned int i = 0; i < koopas.size(); ++i) {
		if (koopas[i] == NULL) {
			koopas[i] = new Koopa();
			koopas[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			koopas[i]->setTileMap(map);
		}
		else {
			koopas[i]->reset();
		}

		koopas[i]->setPosition(pos_koopas[i]);
	}
	*/
	
	int* map_sprites = sprites->getMap();
	int* map_powerups = powerups->getMap();

	for (int j = 0; j < 20; ++j) {
		for (int i = 0; i < 211; ++i) {
			if (map_sprites[j * 211 + i] == 19) {
				Question* aux = new Question();
				aux->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetAnimatedBlocks);
				aux->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
				aux->setTileMap(map);

				if (map_powerups[j * 211 + i] == 23) {
					Mush* aux2 = new Mush();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetPower);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}
				else if (map_powerups[j * 211 + i] == 24) {
					Star* aux2 = new Star();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetPower);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}
				else if (map_powerups[j * 211 + i] == 25) {
					Coin* aux2 = new Coin();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetCoins);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}

				blocks.push_back(aux);

			}
			else if (map_sprites[j * 211 + i] == 2) {
				Brick* aux = new Brick();
				aux->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetAnimatedBlocks);
				aux->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
				aux->setTileMap(map);

				if (map_powerups[j * 211 + i] == 23) {
					// HAS POWER_UP
					aux->set_gift();

					Mush* aux2 = new Mush();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetPower);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}
				else if (map_powerups[j * 211 + i] == 24) {
					// HAS POWER_UP
					aux->set_gift();

					Star* aux2 = new Star();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetPower);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}

				blocks.push_back(aux);
			}
			else if (map_sprites[j * 211 + i] == 17) {
				Goomba* aux = new Goomba();
				aux->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetGoomba);
				aux->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
				aux->setTileMap(map);
				goombas.push_back(aux);
				pos_goombas.push_back(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
			}
			else if (map_sprites[j * 211 + i] == 18) {
				Koopa* aux = new Koopa();
				aux->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetNKoopa, spritesheetCKoopa);
				aux->setPosition(glm::vec2(i * map->getTileSize(), (j - 1) * map->getTileSize()));
				aux->setTileMap(map);
				koopas.push_back(aux);
				pos_koopas.push_back(glm::vec2(i * map->getTileSize(), (j - 1) * map->getTileSize()));
			}
		}
	}

	delete flag;
	flag = NULL;
	flag = new Flag();
	flag->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	flag->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	centerCam = 256.f;
}

void PlayScene::init()
{
	finish = false;
	curr_level = 1;
	game_over_ret = 0;
	time_UP = false;
	lives = 3;
	index = 0;
	index_pk = 0;
	possible_points = { 100.f,200.f,400.f,500.f,800.f,1000.f,2000.f,4000.f,5000.f,8000.f,10000.f };
	possible_points_koopa = { 500.f,800.f,1000.f,2000.f,4000.f,5000.f,8000.f,10000.f };
	active = false;
	ticks = 400.0f;
	points = 0.0f;
	coins = 0;
	star_timer = 0.f;
	points_timer = 0.f;
	inv_timer = 0.f;
	transition_time = 6.f;
	initShaders();
	engine = irrklang::createIrrKlangDevice();
	//engine->play2D("sounds/lvlMusic.wav", true);

	// MARIO TEXTURES
	spritesheetM.loadFromFile("images/small_mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetSM.loadFromFile("images/superMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetSuperStM.loadFromFile("images/superStarMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetSmallStM.loadFromFile("images/smallStarMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetChange.loadFromFile("images/grow_shrink.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// ANIMATED COIN TEXTURE
	spritesheetCoin.loadFromFile("images/coin_iface.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// STATICIFACE TEXTURE
	spritesheetStatic.loadFromFile("images/static_iface.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	// ANIMATED BLOCKS TEXTURE
	spritesheetAnimatedBlocks.loadFromFile("images/block_animations.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// ENEMIES
	spritesheetCKoopa.loadFromFile("images/koopa_shell.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetNKoopa.loadFromFile("images/koopa.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetGoomba.loadFromFile("images/goomba.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// POWERUPS
	spritesheetPower.loadFromFile("images/powerups.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetCoins.loadFromFile("images/dropCoins.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// PARTICLES
	spritesheetParticles.loadFromFile("images/brick_destruction.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// MAP
	tilesheetMap.loadFromFile("images/complete_tileset.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheetMapA1.loadFromFile("images/game_over.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheetMapA2.loadFromFile("images/transition.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheetMapA3.loadFromFile("images/time_up.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// POINTS
	spritesheetPoints.loadFromFile("images/points.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// TEXT
	spritesheetText.loadFromFile("images/text_tileset.png", TEXTURE_PIXEL_FORMAT_RGBA);

	map1 = TileMap::createTileMap("levels/1-1/1-1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, tilesheetMap);
	back1 = TileMap::createTileMap("levels/1-1/1-1b.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, tilesheetMap);
	sprites1 = TileMap::createTileMap("levels/1-1/1-1s.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, tilesheetMap);
	powerups1 = TileMap::createTileMap("levels/1-1/1-1p.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, tilesheetMap);

	map2 = TileMap::createTileMap("levels/1-2/1-2.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, tilesheetMap);
	back2 = TileMap::createTileMap("levels/1-2/1-2b.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, tilesheetMap);
	sprites2 = TileMap::createTileMap("levels/1-2/1-2s.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, tilesheetMap);
	powerups2 = TileMap::createTileMap("levels/1-2/1-2p.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, tilesheetMap);

	map = map1;
	back = back1;
	sprites = sprites1;
	powerups = powerups1;

	game_over = TileMap::createTileMap("levels/Screens/game_over.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, tilesheetMapA1);
	transition = TileMap::createTileMap("levels/Screens/transition.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, tilesheetMapA2);
	time_up = TileMap::createTileMap("levels/Screens/time_up.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, tilesheetMapA3);

	int* map_sprites = sprites->getMap();
	int* map_powerups = powerups->getMap();

	for (int j = 0 ; j < 20 ; ++j) {
		for (int i = 0; i < 211; ++i) {
			if (map_sprites[j * 211 + i] == 19) {
				Question* aux = new Question();
				aux->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetAnimatedBlocks);
				aux->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
				aux->setTileMap(map);

				if (map_powerups[j * 211 + i] == 23) {
					Mush* aux2 = new Mush();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetPower);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}
				else if (map_powerups[j * 211 + i] == 24) {
					Star* aux2 = new Star();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetPower);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}
				else if (map_powerups[j * 211 + i] == 25) {
					Coin* aux2 = new Coin();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetCoins);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}

				blocks.push_back(aux);

			}
			else if (map_sprites[j * 211 + i] == 2) {
				Brick* aux = new Brick();
				aux->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetAnimatedBlocks);
				aux->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
				aux->setTileMap(map);

				if (map_powerups[j * 211 + i] == 23) {
					// HAS POWER_UP
					aux->set_gift();

					Mush* aux2 = new Mush();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetPower);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}
				else if (map_powerups[j * 211 + i] == 24) {
					// HAS POWER_UP
					aux->set_gift();

					Star* aux2 = new Star();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetPower);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}

				blocks.push_back(aux);
			}
			else if (map_sprites[j * 211 + i] == 17) {
				Goomba* aux = new Goomba();
				aux->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetGoomba);
				aux->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
				aux->setTileMap(map);
				goombas.push_back(aux);
				pos_goombas.push_back(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
			}
			else if (map_sprites[j * 211 + i] == 18) {
				Koopa* aux = new Koopa();
				aux->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetNKoopa, spritesheetCKoopa);
				aux->setPosition(glm::vec2(i * map->getTileSize(), (j-1) * map->getTileSize()));
				aux->setTileMap(map);
				koopas.push_back(aux);
				pos_koopas.push_back(glm::vec2(i * map->getTileSize(), (j-1) * map->getTileSize()));
			}
		}
	}
	
	level = new Text();
	level->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	level->setPosition(glm::vec2(21 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	level->setNumber(1);

	world = new Text();
	world->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	world->setPosition(glm::vec2(19 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	world->setNumber(1);

	level_tran = new Text();
	level_tran->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	level_tran->setPosition(glm::vec2(314.f, 198.f));
	level_tran->setNumber(1);

	world_tran = new Text();
	world_tran->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	world_tran->setPosition(glm::vec2(278.f, 198.f));
	world_tran->setNumber(1);

	lives_sp = new Text();
	lives_sp->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	lives_sp->setPosition(glm::vec2(280.f, 260.f));
	lives_sp->setNumber(lives);

	coinSprite = new AnimatedCoin();
	coinSprite->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetCoin);
	coinSprite->setPosition(glm::vec2(11 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));

	staticSprite = new StaticIface();
	staticSprite->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetStatic);
	staticSprite->setPosition(glm::vec2(0.f, map->getTileSize() / 2));

	for (auto & digit : timer) {
		digit = new Text();
	}

	timer[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	timer[0]->setPosition(glm::vec2(26 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	timer[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	timer[1]->setPosition(glm::vec2(27 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	timer[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	timer[2]->setPosition(glm::vec2(28 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));

	for (auto & digit : point_counter) {
		digit = new Text();
	}

	point_counter[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	point_counter[0]->setPosition(glm::vec2(3 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	point_counter[1]->setPosition(glm::vec2(4 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	point_counter[2]->setPosition(glm::vec2(5 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	point_counter[3]->setPosition(glm::vec2(6 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[4]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	point_counter[4]->setPosition(glm::vec2(7 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[5]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	point_counter[5]->setPosition(glm::vec2(8 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));

	for (auto & digit : coin_counter) {
		digit = new Text();
	}

	coin_counter[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	coin_counter[0]->setPosition(glm::vec2(13 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	coin_counter[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetText);
	coin_counter[1]->setPosition(glm::vec2(14 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));


	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetM, spritesheetSM, spritesheetSuperStM, spritesheetSmallStM, spritesheetChange);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	flag = new Flag();
	flag->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	flag->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	centerCam = 256.f;
}

void PlayScene::timer_update(int deltaTime) 
{
	if (ticks > 0.f)
		ticks -= deltaTime / 400.f;

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
}

void PlayScene::transition_timer_update(int deltaTime)
{
	if (transition_time > 0.f)
		transition_time -= deltaTime / 400.f;

	if (transition_time < 0.f || transition_time < 0.005f)
		transition_time = 0.f;
}

void PlayScene::point_counter_update(int deltaTime)
{
	// DIGITS OF POINTS COUNTER
	int d1 = (int(points) / 100000) % 10;
	int d2 = (int(points) / 10000) % 10;
	int d3 = (int(points) / 1000) % 10;
	int d4 = (int(points) / 100) % 10;
	int d5 = (int(points) / 10) % 10;
	int d6 = int(points) % 10;

	// TIMER ACTUALIZATION
	point_counter[0]->setNumber(d1);
	point_counter[1]->setNumber(d2);
	point_counter[2]->setNumber(d3);
	point_counter[3]->setNumber(d4);
	point_counter[4]->setNumber(d5);
	point_counter[5]->setNumber(d6);
}

void PlayScene::coin_counter_update(int deltaTime) {
	int d1 = (int(coins) / 10) % 10;
	int d2 = int(coins) % 10;

	// TIMER ACTUALIZATION
	coin_counter[0]->setNumber(d1);
	coin_counter[1]->setNumber(d2);
}

void PlayScene::timer_update_end(int deltaTime)
{
	if (ticks > 0.f)
		ticks -= 0.125f * deltaTime;

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
}

void PlayScene::point_counter_update_end(int deltaTime)
{ 
	points += 50.f * (50.f * (deltaTime / 400.f));
	// DIGITS OF POINTS COUNTER
	int d1 = (int(points) / 100000) % 10;
	int d2 = (int(points) / 10000) % 10;
	int d3 = (int(points) / 1000) % 10;
	int d4 = (int(points) / 100) % 10;
	int d5 = (int(points) / 10) % 10;
	int d6 = int(points) % 10;

	// TIMER ACTUALIZATION
	point_counter[0]->setNumber(d1);
	point_counter[1]->setNumber(d2);
	point_counter[2]->setNumber(d3);
	point_counter[3]->setNumber(d4);
	point_counter[4]->setNumber(d5);
	point_counter[5]->setNumber(d6);
}

void PlayScene::init_particles(int pos)
{
	Particles* p1 = new Particles();
	p1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetParticles, -2.f, 10.f,1);
	p1->setPosition(glm::fvec2(float(blocks[blocks_in_motion[pos]]->getPosition().x), float(blocks[blocks_in_motion[pos]]->getPosition().y)));
	particles.push_back(p1);

	Particles* p2 = new Particles();
	p2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetParticles, -1.5f, 8.f,1);
	p2->setPosition(glm::fvec2(float(blocks[blocks_in_motion[pos]]->getPosition().x), float(blocks[blocks_in_motion[pos]]->getPosition().y + 24)));
	particles.push_back(p2);

	Particles* p3 = new Particles();
	p3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetParticles, 2.f, 10.f,0);
	p3->setPosition(glm::fvec2(float(blocks[blocks_in_motion[pos]]->getPosition().x + 24), float(blocks[blocks_in_motion[pos]]->getPosition().y)));
	particles.push_back(p3);

	Particles* p4 = new Particles();
	p4->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetParticles, 1.5f, 8.f,0);
	p4->setPosition(glm::fvec2(float(blocks[blocks_in_motion[pos]]->getPosition().x + 24), float(blocks[blocks_in_motion[pos]]->getPosition().y + 24)));
	particles.push_back(p4);
}

void PlayScene::particles_update(int deltaTime)
{

	for (unsigned int i = 0 ; i < particles.size() ; ++i ) {
		if (particles[i] != NULL) {
			particles[i]->update(deltaTime);

			if (particles[i]->get_inactive()) {
				delete particles[i];
				particles[i] = NULL;
				particles.erase(particles.begin() + i);
			}
		}
	}
}

void PlayScene::init_pointsSprites(const glm::vec2 &pos, int anim)
{
	PointsSprite* p1 = new PointsSprite();
	p1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, anim, spritesheetPoints);
	p1->setPosition(pos);
	points_sprites.push_back(p1);
}

void PlayScene::pointsSprite_update(int deltaTime)
{

	for (unsigned int i = 0; i < points_sprites.size(); ++i) {
		if (points_sprites[i] != NULL) {
			points_sprites[i]->update(deltaTime);

			if (points_sprites[i]->get_inactive()) {
				delete points_sprites[i];
				points_sprites[i] = NULL;
				points_sprites.erase(points_sprites.begin() + i);
			}
		}
	}
}

void PlayScene::animated_blocks_update(int deltaTime)
{
	// ANIMATED BLOCKS ACTUALIZATION ///////////////////////////////////////////
	for (auto & block : blocks) {
		if (block != NULL) {
			block->obtainPosPlayer(player->getPosition());
			block->setMarioSpriteSize(player->getMarioSpriteSize());
			block->sprite_update(deltaTime);
		}
	}

	for (int i = blocks.size() - 1; (i >= 0) && !active; --i) {
		if (blocks[i] != NULL && blocks[i]->check_colision() && !player->being_killed()) {
			blocks_in_motion.push_back(i);
			float dist = abs(((blocks[i]->getPosition().x) + 16.f) - ((player->getPosition().x) + 16.f));
			distances.push_back(dist);
		}
	}

	if (blocks_in_motion.size() == 2) {
		active = true;
		if (distances[0] <= distances[1]) {
			if (!engine->isCurrentlyPlaying("sounds/blockCollition.wav")) {
				player->silence();
				engine->play2D("sounds/blockCollition.wav", false, false);			
			}
			bool gift = blocks[blocks_in_motion[0]]->get_gift();
			if (player->getMarioSpriteSize().y != 64 || gift) {

				if (gift) {
					blocks[blocks_in_motion[0]]->set_inactive();
				}

				for (auto & powerUp : power_sprites) {
					if (powerUp != NULL && blocks[blocks_in_motion[0]]->getPosition() == powerUp->getPosition() && powerUp->is_coin()) {
						// POINTS COIN
						points += 200.0f;
						++coins;
						engine->play2D("sounds/coin.wav", false, false);
						powerUp->set_poping(true);
						powerUp->set_render(true);
					}
				}

				blocks[blocks_in_motion[0]]->update(deltaTime);
				if (blocks[blocks_in_motion[0]]->not_bumping()) {
					// POWER_UPS
					for (auto & powerUp : power_sprites) {
						if (powerUp != NULL && blocks[blocks_in_motion[0]]->getPosition() == powerUp->getPosition() && !powerUp->is_coin()) {
							blocks[blocks_in_motion[0]]->set_used();
							powerUp->set_poping(true);
							powerUp->set_render(true);
							engine->play2D("sounds/powerup_appears.wav", false, false);
						}
					}
					if (blocks[blocks_in_motion[0]]->is_question()) {
						blocks[blocks_in_motion[0]]->set_used();
					}
					blocks_in_motion.clear();
					distances.clear();
					active = false;
				}

			}

			else {
				// COLISION SUPER MARIO
				engine->play2D("sounds/blockBreak.wav", false, false);
				points += 50.0f;
				init_particles(0);
				map->modify_position(int(blocks[blocks_in_motion[0]]->getPosition().y), int(blocks[blocks_in_motion[0]]->getPosition().x));
				player->setTileMap(map);
				delete blocks[blocks_in_motion[0]];
				blocks[blocks_in_motion[0]] = NULL;

				blocks_in_motion.clear();
				distances.clear();
				active = false;
			}
		}
		else {
			if (!engine->isCurrentlyPlaying("sounds/blockCollition.wav")) {
				player->silence();
				engine->play2D("sounds/blockCollition.wav", false, false);
			}
			bool gift = blocks[blocks_in_motion[1]]->get_gift();
			if (player->getMarioSpriteSize().y != 64 || gift) {

				if (gift) {
					blocks[blocks_in_motion[1]]->set_inactive();
				}

				for (auto & powerUp : power_sprites) {
					if (powerUp != NULL && blocks[blocks_in_motion[1]]->getPosition() == powerUp->getPosition() && powerUp->is_coin()) {
						// POINTS COIN
						points += 200.0f;
						++coins;
						engine->play2D("sounds/coin.wav", false, false);
						powerUp->set_poping(true);
						powerUp->set_render(true);
					}
				}

				blocks[blocks_in_motion[1]]->update(deltaTime);
				if (blocks[blocks_in_motion[1]]->not_bumping()) {
					// POWER_UPS
					for (auto & powerUp : power_sprites) {
						if (powerUp != NULL && blocks[blocks_in_motion[1]]->getPosition() == powerUp->getPosition() && !powerUp->is_coin()) {
							blocks[blocks_in_motion[1]]->set_used();
							powerUp->set_poping(true);
							powerUp->set_render(true);
							engine->play2D("sounds/powerup_appears.wav", false, false);
						}
					}
					if (blocks[blocks_in_motion[1]]->is_question()) {
						blocks[blocks_in_motion[1]]->set_used();
					}
					blocks_in_motion.clear();
					distances.clear();
					active = false;
				}

			}

			else {
				// COLISION SUPER MARIO
				engine->play2D("sounds/blockBreak.wav", false, false);
				points += 50.0f;
				init_particles(1);
				map->modify_position(int(blocks[blocks_in_motion[1]]->getPosition().y), int(blocks[blocks_in_motion[1]]->getPosition().x));
				player->setTileMap(map);
				delete blocks[blocks_in_motion[1]];
				blocks[blocks_in_motion[1]] = NULL;

				blocks_in_motion.clear();
				distances.clear();
				active = false;
			}
		}
	}

	else if (blocks_in_motion.size() == 1) {
		active = true;
		if (!engine->isCurrentlyPlaying("sounds/blockCollition.wav")) {
			player->silence();
			engine->play2D("sounds/blockCollition.wav", false, false);
		}
		bool gift = blocks[blocks_in_motion[0]]->get_gift();
		if (player->getMarioSpriteSize().y != 64 || gift) {

			if (gift) {
				blocks[blocks_in_motion[0]]->set_inactive();
			}

			for (auto & powerUp : power_sprites) {
				if (powerUp != NULL && blocks[blocks_in_motion[0]]->getPosition() == powerUp->getPosition() && powerUp->is_coin()) {
					// POINTS COIN
					points += 200.0f;
					++coins;
					engine->play2D("sounds/coin.wav", false, false);
					powerUp->set_poping(true);
					powerUp->set_render(true);
				}
			}

			blocks[blocks_in_motion[0]]->update(deltaTime);
			if (blocks[blocks_in_motion[0]]->not_bumping()) {
				// POWER_UPS
				for (auto & powerUp : power_sprites) {
					if (powerUp != NULL && blocks[blocks_in_motion[0]]->getPosition() == powerUp->getPosition() && !powerUp->is_coin()) {
						blocks[blocks_in_motion[0]]->set_used();
						powerUp->set_poping(true);
						powerUp->set_render(true);
						engine->play2D("sounds/powerup_appears.wav", false, false);
					}	
				}
				if (blocks[blocks_in_motion[0]]->is_question()) {
					blocks[blocks_in_motion[0]]->set_used();
				}
				blocks_in_motion.clear();
				distances.clear();
				active = false;
			}

		}

		else {
			// COLISION SUPER MARIO
			engine->play2D("sounds/blockBreak.wav", false, false);
			points += 50.0f;
			init_particles(0);
			map->modify_position(int(blocks[blocks_in_motion[0]]->getPosition().y), int(blocks[blocks_in_motion[0]]->getPosition().x));
			player->setTileMap(map);
			delete blocks[blocks_in_motion[0]];
			blocks[blocks_in_motion[0]] = NULL;

			blocks_in_motion.clear();
			distances.clear();
			active = false;
		}
	}
}

void PlayScene::goombas_update(int deltaTime)
{
	// GOOMBA ACTUALIZATION
	for (auto & goomba : goombas) {
		if (goomba != NULL && (goomba->getPosition().x > centerCam-300.f && goomba->getPosition().x < centerCam+300.f)) {
			goomba->update(deltaTime);

			if (goomba->killed()) {
				delete goomba;
				goomba = NULL;
			}

			else if (goomba->being_killed()) {
				if (!goomba->get_player_murderer()) {
					goomba->set_player_murderer(true);
				}
				else if (goomba->get_jumped()){
					// Player kills goomba
					engine->play2D("sounds/stomp.wav", false, false);
					if ((points_timer != 0.f) && (index != 10))
						++index;
					points += possible_points[index];
					init_pointsSprites(goomba->getPosition(), index);
					points_timer = 2.f;
					
					player->set_small_jump();
					goomba->set_jumped(false);
				}
			}

			else if (goomba->get_starMarioKill()) {
				// StarMario kills goomba
				engine->play2D("sounds/kick.wav", false, false);
				if ((points_timer != 0.f) && (index != 10))
					++index;
				points += possible_points[index];
				init_pointsSprites(goomba->getPosition(), index);
				points_timer = 2.f;
			}

			else {
				if (goomba->playerKilled()) {
					player->killAnimation();
					engine->stopAllSounds();
					if (lives != 1) {
						engine->play2D("sounds/mariodie.wav", false, false);
					}
					else {
						engine->play2D("sounds/gameover.wav", false, false);
					}
					// player = NULL;
					break;
				}
				else if (goomba->isHit()) {
					player->set_Shrinking();
					engine->play2D("sounds/hit.wav", false, false);
					inv_timer = 10.f;
					//player->hit();
					goomba->setHit();
				}
				
				glm::ivec2 pos = player->getPosition();
				goomba->obtainPosPlayer(pos);
				goomba->setMarioSpriteSize(player->getMarioSpriteSize());
				goomba->setStarMario(player->isStarMario());
				goomba->setInvMario(player->get_Invulnerable());

			}
		}
		else if (goomba != NULL && goomba->getPosition().x < centerCam - 300.f) {
			delete goomba;
			goomba = NULL;
		}
	}
}
void PlayScene::koopas_update(int deltaTime)
{
	// KOOPA ACTUALIZATION
	for (auto & koopa : koopas) {
		if (koopa != NULL && (koopa->getPosition().x > centerCam - 300.f && koopa->getPosition().x < centerCam + 300.f)) {
			koopa->update(deltaTime);
			if (koopa->killed()) {
				if (!koopa->get_player_murderer()) {
					koopa->set_player_murderer(true);
				}
				else {
					// STAR MARIO SEGURO!!!
					//player->set_small_jump();
				}
				delete koopa;
				koopa = NULL;
			}

			else if (koopa->get_starMarioKill()) {
				engine->play2D("sounds/kick.wav", false, false);
				if ((points_timer != 0.f) && (index != 10))
					++index;
				points += possible_points[index];
				init_pointsSprites(koopa->getPosition(), index);
				points_timer = 2.f;
			}

			else {
				if (koopa->hitted()) {

					// Koopa hit points
					if (*koopa->getVelocity() == 0.f) {
						engine->play2D("sounds/stomp.wav", false, false);
						if ((points_timer != 0.f) && (index != 10)) {
							++index;
						}
						points += possible_points[index];
						init_pointsSprites(koopa->getPosition(), index);
						points_timer = 2.f;
					}

					else {
						engine->play2D("sounds/kick.wav", false, false);
						points += 400.f;
						init_pointsSprites(koopa->getPosition(), 2);
						points_timer = 2.f;
					}

					player->set_small_jump();
					koopa->disable_hitted();
				}

				else if (koopa->get_shell_hit_side()) {
					engine->play2D("sounds/kick.wav", false, false);
					points += 400.f;
					init_pointsSprites(koopa->getPosition(), 2);
					koopa->unset_shell_hit_side();
					points_timer = 2.f;
				}

				if (koopa->playerKilled()) {
					player->killAnimation();
					engine->stopAllSounds();
					if (lives != 1) {
						engine->play2D("sounds/mariodie.wav", false, false);
					}
					else {
						engine->play2D("sounds/gameover.wav", false, false);
					}
					// player = NULL;
					break;
				}
				else if (koopa->isHit()) {
					player->set_Shrinking();
					engine->play2D("sounds/hit.wav", false, false);
					inv_timer = 10.f;
					//player->hit();
					koopa->setHit();
				}
				

				glm::ivec2 pos = player->getPosition();
				koopa->obtainPosPlayer(pos);
				koopa->setMarioSpriteSize(player->getMarioSpriteSize());
				koopa->setStarMario(player->isStarMario());
				koopa->setInvMario(player->get_Invulnerable());
				
			}
		}

		else if (koopa != NULL && abs(*koopa->getVelocity()) == 5.f){
			delete koopa;
			koopa = NULL;
		}
		else if (koopa != NULL && koopa->getPosition().x < centerCam - 300.f) {
			delete koopa;
			koopa = NULL;
		}
	}
}
void PlayScene::enemy_collisions() 
{
	//// ENEMY COLLISIONS ////////////////////////////////////////////////////////////////////////////////////////////////
	for (auto & goomba : goombas) {
		if (goomba != NULL) {
			for (auto & koopa : koopas) {
				if (koopa != NULL) {
					int aux = map->collisionEnemyEnemy(goomba->getPosition(), glm::ivec2(32, 32), *goomba->getVelocity(),
						koopa->getPosition(), koopa->get_sprite_size(), *koopa->getVelocity());
					switch (aux)
					{
					case 1:
						if (!goomba->get_flipped()) {
							engine->play2D("sounds/kick.wav", false, false);
							if ((points_timer != 0.f) && (index_pk != 10))
								++index_pk;
							points += possible_points_koopa[index_pk];
							init_pointsSprites(goomba->getPosition(), index_pk + 2);
							points_timer = 2.f;

							goomba->set_flipped_death();
						}
						goomba->set_player_murderer(false);
						goomba->setDying();
						break;
					case 2:
						// This is imposible
						koopa->setDying();
						break;
					case -1:
						break;
					}
				}
			}
		}
	}

	for (unsigned int i = 0; i < goombas.size(); ++i) {
		if (goombas[i] != NULL) {
			for (unsigned int j = i + 1; j < goombas.size(); ++j) {
				if (goombas[j] != NULL) {
					map->collisionEnemyEnemy(goombas[i]->getPosition(), glm::ivec2(32, 32), *goombas[i]->getVelocity(),
						goombas[j]->getPosition(), glm::ivec2(32, 32), *goombas[j]->getVelocity());
				}
			}
		}
	}

	for (unsigned int i = 0; i < koopas.size(); ++i) {
		if (koopas[i] != NULL) {
			for (unsigned int j = 0; j < blocks.size(); ++j) {
				if (blocks[j] != NULL) {
					if (map->collisionBlockEnemy(koopas[i]->getPosition(), glm::ivec2(32, 64), blocks[j]->getPosition(), glm::ivec2(32, 32))) {
						if (!koopas[i]->get_flipped()) {
							engine->play2D("sounds/kick.wav", false, false);
							if ((points_timer != 0.f) && (index != 10))
								++index;
							points += possible_points_koopa[index];
							init_pointsSprites(koopas[i]->getPosition(), index);
							points_timer = 2.f;

							koopas[i]->set_flipped_death();
						}
						koopas[i]->set_player_murderer(false);
						koopas[i]->setDying();
					}
				}
			}
		}
	}

	for (unsigned int i = 0; i < goombas.size(); ++i) {
		if (goombas[i] != NULL) {
			for (unsigned int j = 0; j < blocks.size(); ++j) {
				if (blocks[j] != NULL) {
					if (map->collisionBlockEnemy(goombas[i]->getPosition(), glm::ivec2(32, 32), blocks[j]->getPosition(), glm::ivec2(32, 32))) {
						if (!goombas[i]->get_flipped()) {
							engine->play2D("sounds/kick.wav", false, false);
							if ((points_timer != 0.f) && (index != 10))
								++index;
							points += possible_points_koopa[index];
							init_pointsSprites(goombas[i]->getPosition(), index);
							points_timer = 2.f;

							goombas[i]->set_flipped_death();
						}
						goombas[i]->set_player_murderer(false);
						goombas[i]->setDying();
					}
				}
			}
		}
	}

	for (unsigned int i = 0; i < koopas.size(); ++i) {
		if (koopas[i] != NULL) {
			for (unsigned int j = i + 1; j < koopas.size(); ++j) {
				if (koopas[j] != NULL) {
					int aux = map->collisionEnemyEnemy(koopas[i]->getPosition(), koopas[i]->get_sprite_size(), *koopas[i]->getVelocity(),
						koopas[j]->getPosition(), koopas[j]->get_sprite_size(), *koopas[j]->getVelocity());
					switch (aux)
					{
					case 1:
						koopas[i]->setDying();
						if (!koopas[i]->get_flipped()) {
							engine->play2D("sounds/kick.wav", false, false);
							points += possible_points_koopa[index_pk];
							init_pointsSprites(koopas[i]->getPosition(), index_pk + 3);
							points_timer = 2.f;

							if ((points_timer != 0.f) && (index_pk != 10))
								++index_pk;

							koopas[i]->set_flipped_death();
						}
						koopas[i]->set_player_murderer(false);
						break;
					case 2:
						koopas[j]->setDying();
						if (!koopas[j]->get_flipped()) {
							engine->play2D("sounds/kick.wav", false, false);
							points += possible_points_koopa[index_pk];
							init_pointsSprites(koopas[j]->getPosition(), index_pk + 3);
							points_timer = 2.f;

							if ((points_timer != 0.f) && (index_pk != 10))
								++index_pk;

							koopas[j]->set_flipped_death();
						}
						koopas[j]->set_player_murderer(false);
						break;
					case -1:
						break;
					}
				}
			}
		}
	}
}
void PlayScene::camera_update()
{
	// CAMERA ACTUALIZATION
	glm::ivec2 pos = player->getPosition();
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
			for (auto & digit : point_counter) {
				digit->setPosition(glm::fvec2(digit->getPosition().x + v, digit->getPosition().y));
			}
			
			for (auto & digit : coin_counter) {
				digit->setPosition(glm::fvec2(digit->getPosition().x + v, digit->getPosition().y));
			}

			world->setPosition(glm::fvec2(world->getPosition().x + v, world->getPosition().y));
			level->setPosition(glm::fvec2(level->getPosition().x + v, level->getPosition().y));
			coinSprite->setPosition(glm::fvec2(coinSprite->getPosition().x + v, coinSprite->getPosition().y));
			staticSprite->setPosition(glm::fvec2(staticSprite->getPosition().x + v, staticSprite->getPosition().y));
			centerCam += v;
		}

		else if (diff <= 130) {
			projection = glm::translate(projection, glm::vec3(-(v / 2.f), 0.f, 0.f));
			for (auto & digit : timer) {
				digit->setPosition(glm::fvec2(digit->getPosition().x + (v / 2), digit->getPosition().y));
			}
			for (auto & digit : point_counter) {
				digit->setPosition(glm::fvec2(digit->getPosition().x + (v / 2), digit->getPosition().y));
			}
			for (auto & digit : coin_counter) {
				digit->setPosition(glm::fvec2(digit->getPosition().x + (v / 2), digit->getPosition().y));
			}

			world->setPosition(glm::fvec2(world->getPosition().x + (v / 2), world->getPosition().y));
			level->setPosition(glm::fvec2(level->getPosition().x + (v / 2), level->getPosition().y));
			coinSprite->setPosition(glm::fvec2(coinSprite->getPosition().x + (v / 2), coinSprite->getPosition().y));
			staticSprite->setPosition(glm::fvec2(staticSprite->getPosition().x + (v / 2), staticSprite->getPosition().y));
			centerCam += (v / 2.f);
		}
	}
}

void PlayScene::powerUps_update(int deltaTime)
{
	for (auto & powerUp : power_sprites) {
		if (powerUp != NULL && powerUp->get_render()) {
			powerUp->obtainPosPlayer(player->getPosition());
			powerUp->setMarioSpriteSize(player->getMarioSpriteSize());
			powerUp->update(deltaTime);

			if (powerUp->is_picked() == 1) {
				// POINTS POWERUP
				engine->play2D("sounds/getPowerup.wav", false, false);
				points += 1000.0f;
				init_pointsSprites(powerUp->getPosition(), 5);
				if (!player->isSuperMario()) {
					player->set_Growing();
				}
				delete powerUp;
				powerUp = NULL;
			}
			else if (powerUp->is_picked() == 2) {
				// POINTS POWERUP
				engine->stopAllSounds();
				engine->play2D("sounds/getPowerup.wav", false, false);
				points += 1000.0f;
				init_pointsSprites(powerUp->getPosition(), 5);
				player->setStarMarioSprite();
				engine->play2D("sounds/starMario.wav", false, false);
				star_timer = 30.f;
				delete powerUp;
				powerUp = NULL;
			}

			else if (powerUp->is_picked() == 3) {
				init_pointsSprites(glm::fvec2(powerUp->getPosition().x, powerUp->getPosition().y+5), 1);
				delete powerUp;
				powerUp = NULL;
			}
		}
	}
}

void PlayScene::star_timer_update(int deltaTime)
{
	if (!player->is_infinite()) {
		if (star_timer > 0.f)
			star_timer -= deltaTime / 400.f;
		if (star_timer < 5.f) {
			player->setAnimationSpeed();
		}
		if (star_timer < 0.f || star_timer < 0.005f) {
			player->setBackAnimationSpeed();
			if (player->isSuperMario()) {
				player->unsetStarMario();
				player->setSuperMarioSprite();
			}
			else {
				player->setMarioSprite();
			}
			star_timer = 0.f;
		}
	}
}

void PlayScene::points_timer_update(int deltaTime)
{
	if (points_timer > 0.f)
		points_timer -= deltaTime / 400.f;
	if (points_timer < 0.f || points_timer < 0.005f) {
		index = 0;
		index_pk = 0;
		points_timer = 0.f;
	}
}

void PlayScene::inv_timer_update(int deltaTime)
{
	if (inv_timer > 0.f)
		inv_timer -= deltaTime / 400.f;
	if (inv_timer < 0.f || inv_timer < 0.005f) {
		player->unset_Invulnerable();
		inv_timer = 0.f;
	}
}

int PlayScene::update(int deltaTime)
{
	if (Game::instance().getKey('1')) {
		curr_level = 1;
		transition_time = 6.f;
		engine->stopAllSounds();
		reset();
	}
	else if (Game::instance().getKey('2')) {
		curr_level = 2;
		transition_time = 6.f;
		engine->stopAllSounds();
		reset();
	}
	else if (Game::instance().getKey('b')) {
		curr_level = 1;
		transition_time = 6.f;
		engine->stopAllSounds();
		return 1;
	}

	if (transition_time == 0.f) {
		if (lives==0)
			game_over_ret = 1;

		//TRANSITION
		if (player == NULL && lives != 0) {
			transition_time = 6.f;
			--lives;
			lives_sp->setNumber(lives);
			if (ticks == 0 && lives != 0) {
				time_UP = true;
				transition_time = 12.f;
			}
			reset();
			return 0;
		}

		//GAME OVER
		else if (player == NULL && lives == 0) {
			transition_time = 6.f;
			reset();
			return 0;
		}

		time_UP = false;
		currentTime += deltaTime;
		star_timer_update(deltaTime);
		inv_timer_update(deltaTime);

		if (player->being_killed()) {
			player->update(deltaTime);
			animated_blocks_update(deltaTime);
			coinSprite->update(deltaTime);
		}

		else if (player->get_Growing()) {
			player->update(deltaTime);
			animated_blocks_update(deltaTime);
			coinSprite->update(deltaTime);
		}

		else if (player->get_Shrinking()) {
			player->update(deltaTime);
			animated_blocks_update(deltaTime);
			coinSprite->update(deltaTime);
		}

		else if (player->killed()) {
			delete player;
			player = NULL;
		}

		else {

			if (player->getPosition().x <= 6528) {
				player->update(deltaTime);
			}

			if (player->getPosition().y > 512 || (ticks == 0.f && player->getPosition().x <= 6528)) {
				player->killAnimation();
				engine->stopAllSounds();
				if (lives != 1) {
					engine->play2D("sounds/mariodie.wav", false, false);
				}
				else {
					engine->play2D("sounds/gameover.wav", false, false);
				}
			}
			else if (!player->get_inFlag() && !player->getFinalAnimation() && !player->isStarMario()){
				if (!engine->isCurrentlyPlaying("sounds/lvlMusic.wav")) {
					engine->play2D("sounds/lvlMusic.wav", true);
				}
			}

			animated_blocks_update(deltaTime);
			particles_update(deltaTime);
			pointsSprite_update(deltaTime);

			goombas_update(deltaTime);
			koopas_update(deltaTime);
			enemy_collisions();
			camera_update();

			powerUps_update(deltaTime);

			flag->update(deltaTime, player->getPosition());
			if (flag->get_points()) {
				engine->stopAllSounds();
				int center = player->getPosition().y + (player->getMarioSpriteSize().y / 2);
				//CALCULAR PUNTOS
				if (center >= 351) {
					points += 100.0f;
					init_pointsSprites(player->getPosition(), 0);
				}
				else if (center >= 287) {
					points += 400.0f;
					init_pointsSprites(player->getPosition(), 2);
				}
				else if (center >= 223) {
					points += 800.0f;
					init_pointsSprites(player->getPosition(), 4);
				}
				else if (center >= 159) {
					points += 2000.0f;
					init_pointsSprites(player->getPosition(), 6);
				}
				else if (center >= 127) {
					points += 4000.0f;
					init_pointsSprites(player->getPosition(), 7);
				}
				else {
					points += 5000.0f;
					init_pointsSprites(player->getPosition(), 8);
				}
				player->setInFlag();
				flag->unset_points();
			}

			if (player->getPosition().x <= 6528) {
				timer_update(deltaTime);
				point_counter_update(deltaTime);
				coin_counter_update(deltaTime);
			}
			else {
				timer_update_end(deltaTime);
				if (ticks != 0.f) {
					point_counter_update_end(deltaTime);
					if (!engine->isCurrentlyPlaying("sounds/score.wav")) {
						engine->play2D("sounds/score.wav", true);
					}
				}
				else {
					engine->stopAllSounds();
				}
			}

			if (!(player->getPosition().x <= 6528) && !(player->is_final_song()) && curr_level == 1) {
				curr_level = 2;
				transition_time = 6.f;
				engine->stopAllSounds();
				reset();
			}

			if (!(player->getPosition().x <= 6528) && !(player->is_final_song()) && curr_level == 2) {
				curr_level = 1;

				//transition_time = 6.f;
				engine->stopAllSounds();
				finish = true;
				game_over_ret = 1;
			}

			staticSprite->update(deltaTime);
			coinSprite->update(deltaTime);
			points_timer_update(deltaTime);
		}
	}
	transition_timer_update(deltaTime);
	if (game_over_ret == 1) {
		silence();
		curr_level = 1;
	}
	return game_over_ret;
}

void PlayScene::render_iface() {
	for (auto & digit : timer) {
		digit->render();
	}

	for (auto & digit : point_counter) {
		digit->render();
	}

	for (auto & digit : coin_counter) {
		digit->render();
	}

	level->render();

	world->render();

	coinSprite->render();

	staticSprite->render();
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

	if (transition_time == 0.f) {

		back->render();
		map->render();
		flag->render();
		render_iface();

		for (auto & powerUp : power_sprites) {
			if (powerUp != NULL && powerUp->get_render())
				powerUp->render();
		}

		for (auto & block : blocks)
			if (block != NULL) {
				block->render();
			}

		for (auto & particle : particles) {
			if (particle != NULL)
				particle->render();
		}

		for (auto & goomba : goombas) {
			if (goomba != NULL)
				goomba->render();
		}

		for (auto & koopa : koopas) {
			if (koopa != NULL)
				koopa->render();
		}

		if (player != NULL && int(inv_timer * 10000) % 2 == 0 && player->getPosition().x <= 6528)
			player->render();

		for (auto & points_sprite : points_sprites) {
			if (points_sprite != NULL)
				points_sprite->render();
		}
	}
	else if (lives == 0) {
		game_over->render();
	}
	else if (lives != 0 && time_UP) {
		if (transition_time > 6.f) {
			//engine->stopAllSounds();
			time_up->render();
		}
		else {
			//engine->stopAllSounds();
			transition->render();
			level_tran->render();
			world_tran->render();
			lives_sp->render();
		}
	}
	else if (lives != 0) {
		//engine->stopAllSounds();
		transition->render();
		level_tran->render();
		world_tran->render();
		lives_sp->render();
	}
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

void PlayScene::set_lives()
{
	transition_time = 6.f;
	lives = 3;
}

void PlayScene::silence()
{
	engine->stopAllSounds();
}

bool PlayScene::get_Finish()
{
	return finish;
}
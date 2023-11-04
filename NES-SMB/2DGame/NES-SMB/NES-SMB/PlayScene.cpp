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
	sprites = NULL;
	timer.resize(3, nullptr);
	point_counter.resize(6, nullptr);
	player = NULL;
	flag = NULL;
	engine = NULL;
}

PlayScene::~PlayScene()
{
	if (map != NULL)
		delete map;
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

void PlayScene::reset()
{
	blocks_in_motion.clear();
	distances.clear();

	active = false;
	ticks = 400.0f;
	points = 0.0f;
	star_timer = 0.f;
	inv_timer = 0.f;

	timer[0]->setPosition(glm::vec2(25 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	timer[1]->setPosition(glm::vec2(26 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	timer[2]->setPosition(glm::vec2(27 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));

	point_counter[0]->setPosition(glm::vec2(2 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[1]->setPosition(glm::vec2(3 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[2]->setPosition(glm::vec2(4 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[3]->setPosition(glm::vec2(5 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[4]->setPosition(glm::vec2(6 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[5]->setPosition(glm::vec2(7 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));

	//initShaders();
	engine = irrklang::createIrrKlangDevice();
	//engine->play2D("sounds/lvlMusic.ogg", true);

	if (player == NULL) {
		player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spritesheetM, spritesheetSM, spritesheetSuperStM, spritesheetSmallStM, spritesheetChange);
		player->setTileMap(map);
	}
	else {
		player->reset();
	}
	
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));

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

	//flag->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	flag->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	centerCam = 256.f;
}

void PlayScene::init()
{
	active = false;
	ticks = 400.0f;
	points = 0.0f;
	star_timer = 0.f;
	inv_timer = 0.f;
	initShaders();
	engine = irrklang::createIrrKlangDevice();
	//engine->play2D("sounds/lvlMusic.ogg", true);

	// MARIO TEXTURES
	spritesheetM.loadFromFile("images/small_mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetSM.loadFromFile("images/superMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetSuperStM.loadFromFile("images/superStarMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetSmallStM.loadFromFile("images/smallStarMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetChange.loadFromFile("images/grow_shrink.png", TEXTURE_PIXEL_FORMAT_RGBA);

	map = TileMap::createTileMap("levels/1-1/1-1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	back = TileMap::createTileMap("levels/1-1/1-1b.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	sprites = TileMap::createTileMap("levels/1-1/1-1s.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	powerups = TileMap::createTileMap("levels/1-1/1-1p.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	int* map_sprites = sprites->getMap();
	int* map_powerups = powerups->getMap();

	for (int j = 0 ; j < 20 ; ++j) {
		for (int i = 0; i < 211; ++i) {
			if (map_sprites[j * 211 + i] == 19) {
				Question* aux = new Question();
				aux->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				aux->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
				aux->setTileMap(map);

				if (map_powerups[j * 211 + i] == 23) {
					Mush* aux2 = new Mush();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}
				else if (map_powerups[j * 211 + i] == 24) {
					Star* aux2 = new Star();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}
				else if (map_powerups[j * 211 + i] == 25) {
					Coin* aux2 = new Coin();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}

				blocks.push_back(aux);

			}
			else if (map_sprites[j * 211 + i] == 2) {
				Brick* aux = new Brick();
				aux->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				aux->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
				aux->setTileMap(map);

				if (map_powerups[j * 211 + i] == 23) {
					// HAS POWER_UP
					aux->set_gift();

					Mush* aux2 = new Mush();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}
				else if (map_powerups[j * 211 + i] == 24) {
					// HAS POWER_UP
					aux->set_gift();

					Star* aux2 = new Star();
					aux2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					aux2->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
					aux2->setTileMap(map);
					power_sprites.push_back(aux2);
				}

				blocks.push_back(aux);
			}
			else if (map_sprites[j * 211 + i] == 17) {
				Goomba* aux = new Goomba();
				aux->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				aux->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
				aux->setTileMap(map);
				goombas.push_back(aux);
				pos_goombas.push_back(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
			}
			else if (map_sprites[j * 211 + i] == 18) {
				Koopa* aux = new Koopa();
				aux->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				aux->setPosition(glm::vec2(i * map->getTileSize(), (j-1) * map->getTileSize()));
				aux->setTileMap(map);
				koopas.push_back(aux);
				pos_koopas.push_back(glm::vec2(i * map->getTileSize(), (j-1) * map->getTileSize()));
			}
		}
	}

	for (auto & digit : timer) {
		digit = new Text();
	}

	timer[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	timer[0]->setPosition(glm::vec2(25 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	timer[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	timer[1]->setPosition(glm::vec2(26 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	timer[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	timer[2]->setPosition(glm::vec2(27 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));

	for (auto & digit : point_counter) {
		digit = new Text();
	}

	point_counter[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	point_counter[0]->setPosition(glm::vec2(2 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	point_counter[1]->setPosition(glm::vec2(3 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	point_counter[2]->setPosition(glm::vec2(4 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	point_counter[3]->setPosition(glm::vec2(5 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[4]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	point_counter[4]->setPosition(glm::vec2(6 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));
	point_counter[5]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	point_counter[5]->setPosition(glm::vec2(7 * map->getTileSize() / 2, 2 * map->getTileSize() / 2));

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
	p1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, -2.f, 10.f,1);
	p1->setPosition(glm::fvec2(float(blocks[blocks_in_motion[pos]]->getPosition().x), float(blocks[blocks_in_motion[pos]]->getPosition().y)));
	particles.push_back(p1);

	Particles* p2 = new Particles();
	p2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, -1.5f, 8.f,1);
	p2->setPosition(glm::fvec2(float(blocks[blocks_in_motion[pos]]->getPosition().x), float(blocks[blocks_in_motion[pos]]->getPosition().y + 24)));
	particles.push_back(p2);

	Particles* p3 = new Particles();
	p3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2.f, 10.f,0);
	p3->setPosition(glm::fvec2(float(blocks[blocks_in_motion[pos]]->getPosition().x + 24), float(blocks[blocks_in_motion[pos]]->getPosition().y)));
	particles.push_back(p3);

	Particles* p4 = new Particles();
	p4->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1.5f, 8.f,0);
	p4->setPosition(glm::fvec2(float(blocks[blocks_in_motion[pos]]->getPosition().x + 24), float(blocks[blocks_in_motion[pos]]->getPosition().y + 24)));
	particles.push_back(p4);
}

void PlayScene::particles_update(int deltaTime)
{
	for (auto & particle : particles) {
		particle->update(deltaTime);
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
		if (blocks[i] != NULL && blocks[i]->check_colision()) {
			blocks_in_motion.push_back(i);
			float dist = abs(((blocks[i]->getPosition().x) + 16.f) - ((player->getPosition().x) + 16.f));
			distances.push_back(dist);
		}
	}

	if (blocks_in_motion.size() == 2) {
		active = true;
		if (distances[0] <= distances[1]) {

			bool gift = blocks[blocks_in_motion[0]]->get_gift();
			if (player->getMarioSpriteSize().y != 64 || gift) {

				if (gift) {
					blocks[blocks_in_motion[0]]->set_inactive();
				}

				for (auto & powerUp : power_sprites) {
					if (powerUp != NULL && blocks[blocks_in_motion[0]]->getPosition() == powerUp->getPosition() && powerUp->is_coin()) {
						// POINTS COIN
						points += 200.0f;
						blocks[blocks_in_motion[0]]->set_used();
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
						}
					}
					blocks_in_motion.clear();
					distances.clear();
					active = false;
				}

			}

			else {
				// COLISION SUPER MARIO
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

			bool gift = blocks[blocks_in_motion[1]]->get_gift();
			if (player->getMarioSpriteSize().y != 64 || gift) {

				if (gift) {
					blocks[blocks_in_motion[1]]->set_inactive();
				}

				for (auto & powerUp : power_sprites) {
					if (powerUp != NULL && blocks[blocks_in_motion[1]]->getPosition() == powerUp->getPosition() && powerUp->is_coin()) {
						// POINTS COIN
						points += 200.0f;
						blocks[blocks_in_motion[1]]->set_used();
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
						}
					}
					blocks_in_motion.clear();
					distances.clear();
					active = false;
				}

			}

			else {
				// COLISION SUPER MARIO
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

		bool gift = blocks[blocks_in_motion[0]]->get_gift();
		if (player->getMarioSpriteSize().y != 64 || gift) {

			if (gift) {
				blocks[blocks_in_motion[0]]->set_inactive();
			}

			for (auto & powerUp : power_sprites) {
				if (powerUp != NULL && blocks[blocks_in_motion[0]]->getPosition() == powerUp->getPosition() && powerUp->is_coin()) {
					// POINTS COIN
					points += 200.0f;
					blocks[blocks_in_motion[0]]->set_used();
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
					}
				}
				blocks_in_motion.clear();
				distances.clear();
				active = false;
			}

		}

		else {
			// COLISION SUPER MARIO
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
		if (goomba != NULL) {
			goomba->update(deltaTime);
			if (goomba->being_killed()) {
				if (!goomba->get_player_murderer()) {
					goomba->set_player_murderer(true);
				}
				else if (goomba->get_jumped()){
					player->set_small_jump();
					goomba->set_jumped(false);
				}
			}

			else if (goomba->killed()) {
				delete goomba;
				goomba = NULL;
			}

			else {
				if (goomba->playerKilled()) {
					player->killAnimation();
					// player = NULL;
					break;
				}
				else if (goomba->isHit()) {
					player->set_Shrinking();
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
	}
}
void PlayScene::koopas_update(int deltaTime)
{
	// KOOPA ACTUALIZATION
	for (auto & koopa : koopas) {
		if (koopa != NULL) {
			koopa->update(deltaTime);
			if (koopa->killed()) {
				if (!koopa->get_player_murderer()) {
					koopa->set_player_murderer(true);
				}
				else {
					//player->set_small_jump();
				}
				delete koopa;
				koopa = NULL;
			}
			else {
				if (koopa->hitted()) {
					player->set_small_jump();
					koopa->disable_hitted();
				}
				if (koopa->playerKilled()) {
					player->killAnimation();
					// player = NULL;
					break;
				}
				else if (koopa->isHit()) {
					player->set_Shrinking();
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
						goomba->set_flipped_death();
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
			for (unsigned int j = i + 1; j < koopas.size(); ++j) {
				if (koopas[j] != NULL) {
					int aux = map->collisionEnemyEnemy(koopas[i]->getPosition(), koopas[i]->get_sprite_size(), *koopas[i]->getVelocity(),
						koopas[j]->getPosition(), koopas[j]->get_sprite_size(), *koopas[j]->getVelocity());
					switch (aux)
					{
					case 1:
						koopas[i]->setDying();
						if (!koopas[i]->get_flipped()) {
							koopas[i]->set_flipped_death();
						}
						koopas[i]->set_player_murderer(false);
						break;
					case 2:
						koopas[j]->setDying();
						if (!koopas[j]->get_flipped()) {
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
				points += 1000.0f;
				if (!player->isSuperMario()) {
					player->set_Growing();
				}
				delete powerUp;
				powerUp = NULL;
			}
			else if (powerUp->is_picked() == 2) {
				// POINTS POWERUP
				points += 1000.0f;
				player->setStarMarioSprite();
				star_timer = 30.f;
				delete powerUp;
				powerUp = NULL;
			}

			else if (powerUp->is_picked() == 3) {
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
		if (star_timer < 3.f) {
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

	if (player == NULL || Game::instance().getKey('c')) {
		engine->stopAllSounds();
		engine->drop();
		return 1;
	}

	currentTime += deltaTime;
	star_timer_update(deltaTime);
	inv_timer_update(deltaTime);

	if (player->being_killed()) {
		player->update(deltaTime);
		animated_blocks_update(deltaTime);
	}

	else if (player->get_Growing()) {
		player->update(deltaTime);
		animated_blocks_update(deltaTime);
	}

	else if (player->get_Shrinking()) {
		player->update(deltaTime);
		animated_blocks_update(deltaTime);
	}

	else if (player->killed()) {
		delete player;
		player = NULL;
	}

	else {
		
		player->update(deltaTime);

		if (player->getPosition().y > 512 || (ticks == 0.f && player->getPosition().x <= 6528)) {
			player->killAnimation();
		}

		animated_blocks_update(deltaTime);
		particles_update(deltaTime);

		goombas_update(deltaTime);
		koopas_update(deltaTime);
		enemy_collisions();
		camera_update();

		powerUps_update(deltaTime);

		flag->update(deltaTime, player->getPosition());
		if (flag->get_points()) {
			int center = player->getPosition().y + (player->getMarioSpriteSize().y/2);
			//CALCULAR PUNTOS
			if (center >= 351) {
				points += 100.0f;
			}
			else if (center >= 287) {
				points += 400.0f;
			}
			else if (center >= 223) {
				points += 800.0f;
			}
			else if (center >= 159) {
				points += 2000.0f;
			}
			else if (center >= 127) {
				points += 4000.0f;
			}
			else {
				points += 5000.0f;
			}
			player->setInFlag();
			flag->unset_points();
		}

		if (player->getPosition().x <= 6528) {
			timer_update(deltaTime);
			point_counter_update(deltaTime);
		}
		else {
			timer_update_end(deltaTime);
			if (ticks != 0.f) {
				point_counter_update_end(deltaTime);
			}
		}
	}

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

	if (player != NULL && int(inv_timer*10000)%2 == 0 && player->getPosition().x <= 6528)
		player->render();

	for (auto & digit : timer) {
		digit->render();
	}

	for (auto & digit : point_counter) {
		digit->render();
	}

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

	for (auto & goomba : goombas){
		if (goomba != NULL)
			goomba->render();
	}

	for (auto & koopa : koopas) {
		if (koopa != NULL)
			koopa->render();
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
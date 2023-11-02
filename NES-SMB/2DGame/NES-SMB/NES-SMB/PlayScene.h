#ifndef _PLAY_SCENE_INCLUDE
#define _PLAY_SCENE_INCLUDE

#include "GameScene.h"

#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "BlockSprite.h"
#include "Question.h"
#include "Brick.h"
#include "PowerUps.h"
#include "Mush.h"
#include "Star.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Flag.h"
#include "Text.h"
#include "Particles.h"
#include <irrklang.h>


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class PlayScene :
	protected GameScene
{
public:
	PlayScene();
	~PlayScene();

	void init();
	int update(int deltaTime);
	void timer_update(int deltaTime);
	void animated_blocks_update(int deltaTime);
	void goombas_update(int deltaTime);
	void koopas_update(int deltaTime);
	void enemy_collisions();
	void powerUps_update(int deltaTime);
	void init_particles(int pos);
	void particles_update(int deltaTime);
	void camera_update();
	
	void render();
	void reset();

private:
	void initShaders();

private:

	float ticks;
	bool active;
	int animated_block;
	std::vector<int> blocks_in_motion;
	std::vector<float> distances;
	std::vector<Particles*> particles;
	std::vector<Text*> timer;
	std::vector<BlockSprite*> blocks;
	std::vector<PowerUps*> power_sprites;
	std::vector<Koopa*> koopas;
	std::vector<Goomba*> goombas;
	std::vector<glm::ivec2> pos_koopas;
	std::vector<glm::ivec2> pos_goombas;
	TileMap *map, *back, *sprites, *powerups;
	Player *player;
	Texture spritesheetM, spritesheetSM, spritesheetSuperStM, spritesheetSmallStM;
	Flag *flag;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	float delta = 2.f;
	float camDif;
	int velP;
	glm::ivec2 posP;
	float centerCam;
	irrklang::ISoundEngine* engine;
};


#endif // _PLAY_SCENE_INCLUDE


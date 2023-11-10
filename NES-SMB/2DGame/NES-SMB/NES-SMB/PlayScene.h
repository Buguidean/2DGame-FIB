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
#include "Coin.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Flag.h"
#include "Text.h"
#include "Particles.h"
#include "Sprite.h"
#include "AnimatedCoin.h"
#include "PointsSprite.h"
#include "StaticIface.h"
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
	void point_counter_update(int deltaTime);
	void timer_update_end(int deltaTime);
	void point_counter_update_end(int deltaTime);
	void coin_counter_update(int deltaTime);
	void animated_blocks_update(int deltaTime);
	void goombas_update(int deltaTime);
	void koopas_update(int deltaTime);
	void enemy_collisions();
	void powerUps_update(int deltaTime);
	void init_particles(int pos);
	void init_pointsSprites(const glm::vec2 &pos, int anim);
	void particles_update(int deltaTime);
	void pointsSprite_update(int deltaTime);
	void star_timer_update(int deltaTime);
	void inv_timer_update(int deltaTime);
	void points_timer_update(int deltaTime);
	void transition_timer_update(int deltaTime);
	void camera_update();
	void render_iface();

	void render();
	void reset();
	void set_lives();
	void silence();
	void clean_up();
	bool get_Finish();

private:
	void initShaders();

private:

	float points;
	float star_timer;
	float points_timer;
	float inv_timer;
	float ticks;
	float transition_time;
	bool active;
	bool time_UP;
	int coins;
	int animated_block;
	int index;
	int index_pk;

	bool finish;
	
	std::vector<float> possible_points_koopa;
	std::vector<float> possible_points;
	std::vector<int> blocks_in_motion;
	std::vector<float> distances;
	std::vector<PointsSprite*> points_sprites;
	std::vector<Particles*> particles;
	std::vector<Text*> timer;
	std::vector<Text*> point_counter;
	std::vector<Text*> coin_counter;
	std::vector<BlockSprite*> blocks;
	std::vector<PowerUps*> power_sprites;
	std::vector<Koopa*> koopas;
	std::vector<Goomba*> goombas;
	std::vector<glm::ivec2> pos_koopas;
	std::vector<glm::ivec2> pos_goombas;

	TileMap *map, *back, *sprites, *powerups;
	TileMap *map1, *map2, *back1, *back2, *sprites1, *sprites2, *powerups1, *powerups2;
	TileMap *game_over, *time_up, *transition;
	Player *player;
	Texture spritesheetM, spritesheetSM, spritesheetSuperStM, spritesheetSmallStM, spritesheetChange, spritesheetCoin, spritesheetStatic, spritesheetClamped, spritesheetAnimatedBlocks;
	Texture spritesheetCKoopa, spritesheetNKoopa, spritesheetGoomba;
	Texture spritesheetPower, spritesheetCoins, spritesheetParticles;
	Texture tilesheetMap, tilesheetMapA1, tilesheetMapA2, tilesheetMapA3;
	Texture spritesheetText, spritesheetPoints;
	AnimatedCoin* coinSprite;
	StaticIface* staticSprite;
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

	Text* level;
	Text* world;

	Text* level_tran;
	Text* world_tran;

	Text* lives_sp;

	int lives;
	int curr_level;
	int game_over_ret;
};


#endif // _PLAY_SCENE_INCLUDE


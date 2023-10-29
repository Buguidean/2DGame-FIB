#ifndef _PLAY_SCENE_INCLUDE
#define _PLAY_SCENE_INCLUDE

#include "GameScene.h"

#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "BlockSprite.h"
#include "Question.h"
#include "Brick.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Flag.h"
#include "Text.h"
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
	void render();

private:
	void initShaders();

private:

	float ticks;

	std::vector<Text*> timer;
	std::vector<BlockSprite*> blocks;
	TileMap *map;
	TileMap *back;
	TileMap *sprites;
	Player *player;
	Goomba *goomba;
	Koopa *koopa;
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


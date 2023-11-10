#ifndef _MAIN_SCENE_INCLUDE
#define _MAIN_SCENE_INCLUDE

#include "GameScene.h"
#include "TileMap.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include "Sprite.h"
#include <irrklang.h>

class MainScene :
	protected GameScene
{
public:
	MainScene();
	~MainScene();
	void init();
	int update(int deltaTime);
	void render();
	void reset();
	void set_timer();

private:
	void initShaders();

private:
	void spriteArrowMovement();
	void transition_timer_update(int deltaTime);
	float transition_time;

	TileMap *map,*end;
	ShaderProgram texProgram;
	glm::vec2 posArrow;
	float currentTime;
	float centerCam;
	glm::mat4 projection;
	Texture spritesheetArrow;
	Texture tilesheetMap1, tilesheetMap2;
	Sprite *spriteArrow;
	irrklang::ISoundEngine* engine;

	//
	int counter;
	//
};

#endif // _MAIN_SCENE_INCLUDE
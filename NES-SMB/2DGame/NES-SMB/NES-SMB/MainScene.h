#ifndef _MAIN_SCENE_INCLUDE
#define _MAIN_SCENE_INCLUDE

#include "GameScene.h"
#include "TileMap.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include "Sprite.h"

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

private:
	void initShaders();

private:
	void spriteArrowMovement();

	TileMap *map;
	ShaderProgram texProgram;
	glm::vec2 posArrow;
	float currentTime;
	float centerCam;
	glm::mat4 projection;
	Texture spritesheetArrow;
	Sprite *spriteArrow;

	//
	int counter;
	//
};

#endif // _MAIN_SCENE_INCLUDE
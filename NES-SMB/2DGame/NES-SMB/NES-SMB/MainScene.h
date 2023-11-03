#ifndef _MAIN_SCENE_INCLUDE
#define _MAIN_SCENE_INCLUDE

#include "GameScene.h"
#include "TileMap.h"
#include "ShaderProgram.h"

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
	TileMap *map;
	ShaderProgram texProgram;
	float currentTime;
	float centerCam;
	glm::mat4 projection;
};

#endif // _MAIN_SCENE_INCLUDE
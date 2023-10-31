#ifndef _MAIN_SCENE_INCLUDE
#define _MAIN_SCENE_INCLUDE

#include "GameScene.h"

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
};

#endif // _MAIN_SCENE_INCLUDE
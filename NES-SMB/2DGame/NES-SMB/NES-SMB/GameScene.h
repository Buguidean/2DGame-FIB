#ifndef _GAME_SCENE_INCLUDE
#define _GAME_SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"

class GameScene
{
public:
	GameScene();
	~GameScene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
};

#endif // _GAME_SCENE_INCLUDE

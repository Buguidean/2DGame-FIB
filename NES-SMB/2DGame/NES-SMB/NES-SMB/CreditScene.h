#ifndef _CREDIT_SCENE_INCLUDE
#define _CREDIT_SCENE_INCLUDE

#include "GameScene.h"
#include "TileMap.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>

class CreditScene :
	protected GameScene
{
public:
	CreditScene();
	~CreditScene();
	void init();
	int update(int deltaTime);
	void render();

private:
	
	void initShaders();
	TileMap *map;
	ShaderProgram texProgram;
	glm::mat4 projection;
};

#endif // _CREDIT_SCENE_INCLUDE

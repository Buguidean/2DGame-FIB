#ifndef _INSTRUCTION_SCENE_INCLUDE
#define _INSTRUCTION_SCENE_INCLUDE

#include "GameScene.h"
#include "TileMap.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>

class InstructionScene :
	protected GameScene
{
public:
	InstructionScene();
	~InstructionScene();
	void init();
	int update(int deltaTime);
	void render();

private:
	void initShaders();
	TileMap *map;
	Texture tilesheetMap;
	ShaderProgram texProgram;
	glm::mat4 projection;
};

#endif // _INSTRUCTION_SCENE_INCLUDE

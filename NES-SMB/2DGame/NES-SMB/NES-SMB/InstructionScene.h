#ifndef _INSTRUCTION_SCENE_INCLUDE
#define _INSTRUCTION_SCENE_INCLUDE

#include "GameScene.h"

class InstructionScene :
	protected GameScene
{
public:
	InstructionScene();
	~InstructionScene();

private:
	void initShaders();
};

#endif // _INSTRUCTION_SCENE_INCLUDE

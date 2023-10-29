#ifndef _BRICK_INCLUDE
#define _BRICK_INCLUDE


#include "BlockSprite.h"


// Text is basically a Sprite that represents the texts.

class Brick :
	public BlockSprite
{
	public:
		void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) override;
		void update(int deltaTime) override;
};


#endif // _BRICK_INCLUDE

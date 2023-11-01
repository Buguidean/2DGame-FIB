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
		bool not_bumping() override;
		void sprite_update(int deltaTime) override;
		bool check_colision() override;

	private:
		float vy;
		bool bumping;
		bool drop;
		float startY;
};


#endif // _BRICK_INCLUDE

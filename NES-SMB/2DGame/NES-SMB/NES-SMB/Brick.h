#ifndef _BRICK_INCLUDE
#define _BRICK_INCLUDE


#include "BlockSprite.h"


// Text is basically a Sprite that represents the texts.

class Brick :
	public BlockSprite
{
	public:
		void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &animated_blocks) override;
		void update(int deltaTime) override;
		bool not_bumping() override;
		void sprite_update(int deltaTime) override;
		bool check_colision() override;
		void set_used() override;
		void set_gift() override;
		bool get_gift() override;
		void set_inactive() override;
		bool is_question() override;

	private:
		bool question;
		bool gift;
		float vy;
		bool bumping;
		bool drop;
		bool used;
		float startY;
};


#endif // _BRICK_INCLUDE

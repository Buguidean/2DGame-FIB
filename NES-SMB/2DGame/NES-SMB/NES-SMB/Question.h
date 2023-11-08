#ifndef _QUESTION_INCLUDE
#define _QUESTION_INCLUDE


#include "BlockSprite.h"


// Text is basically a Sprite that represents the texts.

class Question:
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
		bool gift;
		float vy;
		bool question;
		bool bumping;
		bool drop;
		bool used;
		float startY;
};


#endif // _QUESTION_INCLUDE

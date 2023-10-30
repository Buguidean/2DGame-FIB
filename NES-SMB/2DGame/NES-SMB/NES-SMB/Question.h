#ifndef _QUESTION_INCLUDE
#define _QUESTION_INCLUDE


#include "BlockSprite.h"


// Text is basically a Sprite that represents the texts.

class Question:
	public BlockSprite
{
	public:
		void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) override;
		void update(int deltaTime) override;
		bool not_bumping() override;
		void sprite_update(int deltaTime) override;


	private:
		float vy;
		bool bumping;
		float startY;
};


#endif // _QUESTION_INCLUDE

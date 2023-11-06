#ifndef _ANIMATED_COIN_INCLUDE
#define _ANIMATED_COIN_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Text is basically a Sprite that represents the texts.

class AnimatedCoin
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spriteSheetCoin);
	void update(int deltaTime);
	void setPosition(const glm::vec2 &pos);
	glm::fvec2 getPosition();
	void render();


private:
	glm::fvec2 tileMapDispl, posCoin;
	Sprite *sprite;
};


#endif // ANIMATED_COIN_INCLUDE
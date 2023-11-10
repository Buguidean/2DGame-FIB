#ifndef _POINTS_SPRITE_INCLUDE
#define _POINTS_SPRITE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Text is basically a Sprite that represents the texts.

class PointsSprite
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int animation, Texture& spritesheet);
	void update(int deltaTime);
	void setPosition(const glm::vec2 &pos);
	glm::fvec2 getPosition();
	void render();
	bool get_inactive();

private:
	float vy;
	int startY;
	bool inactive;
	glm::fvec2 tileMapDispl, posPoints;
	Sprite *sprite;
};


#endif // _POINTS_SPRITE_INCLUDE
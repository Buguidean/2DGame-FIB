#ifndef _TEXT_INCLUDE
#define _TEXT_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Text is basically a Sprite that represents the texts.

class Text
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPosition();
	float getVelocity();
	void setVelocity();
	bool moving();
	bool moving_up();
	void margin(bool value, int center);
	void killAnimation();

private:
	bool bJumping;
	bool Moving;
	bool marg;
	float vx;
	float vy;
	bool apex;
	int ic;
	glm::ivec2 tileMapDispl, posPlayer, oldPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _TEXT_INCLUDE



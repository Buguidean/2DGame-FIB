#ifndef _POWER_UPS_INCLUDE
#define _POWER_UPS_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Text is basically a Sprite that represents the texts.

class PowerUps
{

public:
	virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	virtual void update(int deltaTime);
	virtual void set_render(const bool state);
	virtual bool get_render();
	virtual void set_poping(const bool state);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::fvec2 getPosition();

	void obtainPosPlayer(const glm::ivec2 &pos);
	void setMarioSpriteSize(glm::ivec2 &size);

protected:
	glm::ivec2 marioSpriteSize, playerPos;
	glm::ivec2 tileMapDispl, posPower;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _POWER_UPS_INCLUDE

#ifndef _STATIC_IFACE_INCLUDE
#define _STATIC_IFACE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Text is basically a Sprite that represents the texts.

class StaticIface
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spriteSheetCoin);
	void update(int deltaTime);
	void setPosition(const glm::vec2 &pos);
	glm::fvec2 getPosition();
	void render();


private:
	glm::fvec2 tileMapDispl, posStatic;
	Sprite *sprite;
};


#endif // _STATIC_IFACE_INCLUDE
#ifndef _FLAG_INCLUDE
#define _FLAG_INCLUDE

#include <glm/glm.hpp>
#include "Sprite.h"
#include "TileMap.h"

class Flag
{
public:
	Flag();
	~Flag();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, glm::ivec2 posMario);
	void setPosition(const glm::vec2 &pos);
	void setTileMap(TileMap *tileMap);
	void setMario();
	void render();
	glm::ivec2 getPosition();
	bool getIsMario();
	bool get_points();
	void unset_points();

private:
	glm::ivec2 tileMapDispl, flagPosition;
	Texture spritesheet;
	Sprite *sprite;
	bool isMario;
	bool points;
	TileMap *map;


};

#endif // _FLAG_INCLUDE

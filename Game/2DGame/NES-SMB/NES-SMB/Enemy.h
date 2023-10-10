#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Enemy is basically a Sprite that represents the enemy. As such it has
// all properties it needs to track its movement, and collisions.


class Enemy
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void obtainPosPlayer(const glm::ivec2 &pos);
	bool killed();
	bool playerKilled();
	glm::ivec2 getPosition();


private:

	float v;
	bool dead;
	bool dead_player;
	glm::ivec2 tileMapDispl, posEnemy, playerPos, oldEnemy;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _ENEMY_INCLUDE

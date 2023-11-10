#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Enemy is basically a Sprite that represents the enemy. As such it has
// all properties it needs to track its movement, and collisions.


class Enemy
{

public:

	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void obtainPosPlayer(const glm::ivec2 &pos);
	bool killed();
	bool being_killed();
	bool playerKilled();
	glm::ivec2 getPosition();
	void setMarioSpriteSize(glm::ivec2 &size);
	void setStarMario(bool s);
	void setInvMario(bool s);
	bool isHit();
	void setHit();
	float* getVelocity();
	bool get_player_murderer();
	bool get_starMarioKill();
	void set_player_murderer(const bool &state);

protected:

	float vx, vy;
	int start_jump;
	bool player_murderer;
	bool dead;
	bool dying;
	bool dead_player;
	bool starMario;
	bool invinc;
	bool hit;
	bool starMarioKill;
	int startY;

	glm::ivec2 tileMapDispl, posEnemy, playerPos, oldEnemy;
	Sprite *sprite;
	TileMap *map;
	glm::ivec2 marioSpriteSize;


};


#endif // _ENEMY_INCLUDE

#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <irrklang.h>


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPosition();
	glm::ivec2 getMarioSpriteSize();
	float getVelocity();
	void setVelocity();
	bool moving();
	bool moving_up();
	void margin(bool value, int center);
	void killAnimation();

	bool isStarMario();
	void setInFlag();
	bool getFinalAnimation();
	void hit();
	void set_small_jump();
	void reset();
	bool killed();
	bool being_killed();
	
private:
	void flagTreatment();
	void getOut(int deltaTime);

	void setMarioSprite();
	void setSuperMarioSprite();
	void setStarMarioSprite();

	bool bJumping;
	bool dead;
	bool dying;
	bool small_jump;
	int death_jump;
	int start_small_jump;
	bool Moving;
	bool marg;
	float vx;
	float vy;
	bool apex;
	int ic;

	glm::ivec2 tileMapDispl, posPlayer, oldPlayer;
	int jumpAngle, startY;
	Texture spritesheetM, spritesheetSM, spritesheetSuperStM, spritesheetSmallStM;
	Sprite *spriteM, *spriteSM, *spriteSuperStM, *spriteSmallStM, *sprite;
	TileMap *map;

	bool inFlag;
	int countAnimation;
	bool finalAnimation;

	bool superMario;
	bool starMario;
	glm::ivec2 spriteSize;
	irrklang::ISoundEngine* engine;

};


#endif // _PLAYER_INCLUDE

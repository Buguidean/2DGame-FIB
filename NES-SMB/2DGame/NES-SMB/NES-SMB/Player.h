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
	~Player();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spritesheetM, Texture &spritesheetSM, Texture &spritesheetSuperStM, Texture &spritesheetSmallStM, Texture &spritesheetChange);
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
	bool isSuperMario();
	void unsetStarMario();
	void setInFlag();
	bool getFinalAnimation();
	void hit();
	void set_small_jump();
	void reset();
	bool killed();
	bool being_killed();
	bool is_infinite();

	void setAnimationSpeed();
	void setBackAnimationSpeed();
	void setMarioSprite();
	void setSuperMarioSprite();
	void setStarMarioSprite();
	void set_Growing();
	bool get_Growing();
	bool get_inFlag();
	void set_Shrinking();
	bool get_Shrinking();
	void unset_Invulnerable();
	bool get_Invulnerable();
	void silence();
	bool is_final_song();
	
private:
	void flagTreatment(int deltaTime);
	void getOut(int deltaTime);

	int counter;
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
	Sprite *spriteM, *spriteSM, *spriteSuperStM, *spriteSmallStM, *spriteChange, *sprite;
	TileMap *map;

	bool inFlag;
	int countAnimation;
	bool finalAnimation;

	bool superMario;
	bool starMario;
	glm::ivec2 spriteSize;
	irrklang::ISoundEngine* engine;

	bool infinite;
	bool growing;
	bool shrinking;
	bool invulnerable;
};


#endif // _PLAYER_INCLUDE

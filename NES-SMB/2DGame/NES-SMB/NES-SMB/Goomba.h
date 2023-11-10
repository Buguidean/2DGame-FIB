#ifndef _GOOMBA_INCLUDE
#define _GOOMBA_INCLUDE

#include "Enemy.h"

class Goomba :
	public Enemy
{
public:
	~Goomba();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spritesheetGoomba);
	void update(int deltaTime);
	void reset();
	void setDying();
	bool get_jumped();
	bool get_flipped();
	void set_jumped(const bool state);
	void set_flipped_death();

private:
	int counter;
	bool jumped;
	bool flipped;
};

#endif // _GOOMBA_INCLUDE

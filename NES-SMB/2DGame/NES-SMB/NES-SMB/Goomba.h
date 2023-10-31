#ifndef _GOOMBA_INCLUDE
#define _GOOMBA_INCLUDE

#include "Enemy.h"

class Goomba :
	public Enemy
{
public:
	~Goomba();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void reset();
	void setDying();

private:
	bool dying;
};

#endif // _GOOMBA_INCLUDE

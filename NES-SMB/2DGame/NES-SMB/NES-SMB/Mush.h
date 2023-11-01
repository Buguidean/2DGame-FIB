#ifndef _MUSH_INCLUDE
#define _MUSH_INCLUDE


#include "PowerUps.h"


// Text is basically a Sprite that represents the texts.

class Mush :
	public PowerUps
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) override;
	void update(int deltaTime) override;
	void set_render(const bool state) override;
	bool get_render() override;
	void set_poping(const bool state) override;

private:
	float vx,vy;
	int startY;
	bool should_render;
	bool poping;
};


#endif // STAR_INCLUDE
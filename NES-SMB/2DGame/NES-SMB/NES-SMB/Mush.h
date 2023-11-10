#ifndef _MUSH_INCLUDE
#define _MUSH_INCLUDE


#include "PowerUps.h"


// Text is basically a Sprite that represents the texts.

class Mush :
	public PowerUps
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spritesheet) override;
	void update(int deltaTime) override;
	void set_render(const bool state) override;
	bool get_render() override;
	void set_poping(const bool state) override;
	int is_picked() override;
	bool is_coin() override;

private:
	float vx,vy;
	int startY;
	bool should_render;
	bool poping;
	int picked;
	bool coin;
};


#endif // STAR_INCLUDE
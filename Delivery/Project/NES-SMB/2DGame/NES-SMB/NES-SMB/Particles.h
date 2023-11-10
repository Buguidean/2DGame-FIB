#ifndef _PARTICLES_INCLUDE
#define _PARTICLES_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Text is basically a Sprite that represents the texts.

class Particles
{
	public:
		void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spritesheet,  float x_vel, float y_vel, int animation);
		void update(int deltaTime);
		void setPosition(const glm::vec2 &pos);
		void render();
		bool get_inactive();

	private:
		float vx, vy;
		int startY;
		bool inactive;
		glm::fvec2 tileMapDispl, posParticle;
		Sprite *sprite;
};


#endif // PARTICLES_INCLUDE
#ifndef _KOOPA_INCLUDE
#define _KOOPA_INCLUDE

#include "Enemy.h"

class Koopa :
	public Enemy
{
public:
	~Koopa();
	
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spritesheetNKoopa, Texture &spritesheetCKoopa);
	void update(int deltaTime);
	bool hitted();
	void disable_hitted();
	void reset();
	void setDying();
	glm::ivec2 get_sprite_size();
	void set_flipped_death();
	bool get_flipped();
	bool get_shell_hit_side();
	void unset_shell_hit_side();


private:
	bool shell_hit_side;
	bool flipped;
	void normal_sprite();
	void shell_sprite();
	bool shield;
	int shieldState;
	int shieldCount;
	bool transitionState;
	bool first_hit;
	glm::ivec2 sprite_size;
	Sprite *spriteN;
	Sprite *spriteC;

	// shieldState = 0: No shield (se usa shield)
	// shieldState = 1: saltas dentro de Koopa y se pone el caparazon
	// shieldState = 2: sales fuera del caparazón
	// shieldState = 3: le pegas por un lateral al caparazón estas dentro
	// shieldState = 4: sales del caparazon => esta en modo matar

	// transitionState: needed when jumping, because in the same jump, it detects the koopa twice or more,
	//with this, it only do once
};

#endif // _KOOPA_INCLUDE
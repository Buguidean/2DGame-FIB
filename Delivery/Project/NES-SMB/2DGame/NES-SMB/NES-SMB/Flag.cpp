#include "Flag.h"

#define INIT_FLAG_X_TILES 197
#define INIT_FLAG_Y_TILES 3
#define downVelocity 4
#define bottomFlag 11 * 32

enum flagAnim
{
	STAND_LEFT
};

Flag::Flag()
{
}


Flag::~Flag()
{
	if (sprite != NULL)
		delete sprite;
}

void Flag::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	flagPosition = glm::ivec2(INIT_FLAG_X_TILES * 32, INIT_FLAG_Y_TILES * 32);
	spritesheet.loadFromFile("images/flag.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(STAND_LEFT, 4);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + flagPosition.x + 16), float(tileMapDispl.y + flagPosition.y)));
	isMario = false;
	points = false;
}

void Flag::update(int deltaTime, glm::ivec2 posMario)
{
	sprite->update(deltaTime);

	if ((!isMario) && (posMario.x >= flagPosition.x + 15)) {
		isMario = true;
		points = true;
	}
	
	if (isMario) {
		if (flagPosition.y + downVelocity < bottomFlag) {
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + flagPosition.x + 16), float(tileMapDispl.y + flagPosition.y + downVelocity)));
			flagPosition.y = flagPosition.y + downVelocity;
		}
		else {
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + flagPosition.x + 16), float(tileMapDispl.y + bottomFlag)));
			//isMario = false;
		}
	}
}

void Flag::setPosition(const glm::vec2 &pos)
{
	flagPosition = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + flagPosition.x), float(tileMapDispl.y + flagPosition.y)));
}

void Flag::setMario() {
	isMario = true;
}

void Flag::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Flag::render()
{
	sprite->render();
}

glm::ivec2 Flag::getPosition() {
	return flagPosition;
}

bool Flag::getIsMario() {
	return isMario;
}

bool Flag::get_points() {
	return points;
}

void Flag::unset_points() {
	points = false;
}

#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include <unordered_set>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, Texture &tilesheetOrig);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, Texture &tilesheetOrig);

	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, int *posX, bool margin, int center) const;
	bool collisionMoveLeftEntity(const glm::ivec2 &pos, const glm::ivec2 &size, int *posX) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, int *posX) const;
	bool collisionMoveRightEntity(const glm::ivec2 &pos, const glm::ivec2 &size, int *posX) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	pair<int, float> collisionMarioEnemy(const glm::ivec2 &posM, const glm::ivec2 &sizeM, const glm::ivec2 &posE, const glm::ivec2 &sizeE) const;
	int collisionMarioBlock(const glm::ivec2 &posM, const glm::ivec2 &sizeM, const glm::ivec2 &posE, const glm::ivec2 &sizeE) const;
	int collisionEnemyEnemy(const glm::ivec2 posE_1, const glm::ivec2 sizeE_1, float &v1, const glm::ivec2 posE_2, const glm::ivec2 sizeE_2, float &v2) const;
	bool collisionBlockEnemy(const glm::ivec2 posE_1, const glm::ivec2 sizeE_1, const glm::ivec2 posE_2, const glm::ivec2 sizeE_2) const;

	int* getMap();
	void modify_position(int j, int i);
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
};


#endif // _TILE_MAP_INCLUDE



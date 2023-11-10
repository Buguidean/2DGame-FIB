#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "TileMap.h"


#define M_PI 3.14159265358979323846f


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, Texture &tilesheetOrig)
{
	TileMap *map = new TileMap(levelFile, minCoords, program, tilesheetOrig);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, Texture &tilesheetOrig)
{
	tilesheet = tilesheetOrig;
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	//tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	int i = 0;
	int j = 0;
	while (std::getline(fin, line)) {
		std::stringstream ss(line);
		std::string token;
		j = 0;
		while (std::getline(ss, token, ',')) {
			int number;
			std::istringstream(token) >> number;
			map[i*mapSize.x + j] = number;
			++j;
		}
		++i;
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				texCoordTile[0] += 2e-4;
				texCoordTile[1] -= 2e-4;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeftEntity(const glm::ivec2 &pos, const glm::ivec2 &size, int *posX) const
{
	int x, y0, y1;

	x = (pos.x + 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 3) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if ((map[y*mapSize.x + x] != 0))
		{
			if (*posX - tileSize * x - size.x <= 5)
			{
				*posX = tileSize * x + size.x;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, int *posX, bool margin, int center) const
{
	int x, y0, y1;
	int size_x = size.x - 5;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 3) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if ( (map[y*mapSize.x + x] != 0) || margin)
		{
			if (margin) {
				*posX = center - 256;
				return true;
			}

			else if (*posX - tileSize * x - size_x <= 10 )
			{
				*posX = tileSize * x + size_x;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveRightEntity(const glm::ivec2 &pos, const glm::ivec2 &size, int *posX) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 3) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y*mapSize.x + x] != 0)
		{
			if (*posX - tileSize * x + size.x <= 5)
			{
				*posX = tileSize * x - size.x;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, int *posX) const
{
	int x, y0, y1;
	int size_x = size.x - 5;
	
	x = (pos.x + size_x) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 3) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if (map[y*mapSize.x + x] != 0)
		{
			if (*posX - tileSize * x + size_x <= 10)
			{
				*posX = tileSize * x - size_x;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;
	int size_x = size.x - 5;

	x0 = (pos.x + 5) / tileSize;
	x1 = (pos.x + size_x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if (map[y*mapSize.x + x] != 0)
		{
			if(*posY - tileSize * y + size.y <= 10)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;
	int size_x = size.x - 5;

	x0 = (pos.x + 5) / tileSize;
	x1 = (pos.x + size_x - 1) / tileSize;
	y = (pos.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] != 0)
		{
			if (*posY - tileSize * y - size.y <= 10)
			{
				*posY = tileSize * y + 32;
				return true;
			}
		}
	}

	return false;
}

int TileMap::collisionEnemyEnemy(const glm::ivec2 posE_1, const glm::ivec2 sizeE_1, float &v1, const glm::ivec2 posE_2, const glm::ivec2 sizeE_2, float &v2) const
{
	int min_xE_1, min_yE_1, max_xE_1, max_yE_1;
	int min_xE_2, min_yE_2, max_xE_2, max_yE_2;

	glm::ivec2 center_M, center_E;

	min_xE_1 = posE_1.x;
	max_xE_1 = (posE_1.x + sizeE_1.x);
	min_yE_1 = posE_1.y;
	max_yE_1 = posE_1.y + sizeE_1.y;

	min_xE_2 = posE_2.x;
	max_xE_2 = posE_2.x + sizeE_2.x;

	min_yE_2 = posE_2.y - 1;
	max_yE_2 = posE_2.y + sizeE_2.y + 1;


	if ((min_xE_1 < max_xE_2) && (min_xE_2 < max_xE_1) && (min_yE_1 < max_yE_2) && (min_yE_2 < max_yE_1)) {
		if (abs(v1) == abs(v2)) {
			v1 = -v1;
			v2 = -v2;
		}

		else if (abs(v1) == 5.f) {
			return 2;
		}

		else if (abs(v2) == 5.f) {
			return 1;
		}

		else if (v1 == 0.0f) {
			v2 = -v2;
		}

		else if (v2 == 0.0f) {
			v1 = -v1;
		}
	}

	return -1;
}

bool TileMap::collisionBlockEnemy(const glm::ivec2 posE_1, const glm::ivec2 sizeE_1, const glm::ivec2 posE_2, const glm::ivec2 sizeE_2) const
{
	int min_xE_1, min_yE_1, max_xE_1, max_yE_1;
	int min_xE_2, min_yE_2, max_xE_2, max_yE_2;

	glm::ivec2 center_M, center_E;

	min_xE_1 = posE_1.x;
	max_xE_1 = (posE_1.x + sizeE_1.x);
	min_yE_1 = posE_1.y;
	max_yE_1 = posE_1.y + sizeE_1.y;

	min_xE_2 = posE_2.x;
	max_xE_2 = posE_2.x + sizeE_2.x;

	min_yE_2 = posE_2.y;
	max_yE_2 = posE_2.y + sizeE_2.y;


	if ((min_xE_1 < max_xE_2) && (min_xE_2 < max_xE_1) && (min_yE_1 < max_yE_2) && (min_yE_2 < max_yE_1)) {
		return true;
	}

	return false;
}

int TileMap::collisionMarioBlock(const glm::ivec2 &posM, const glm::ivec2 &sizeM, const glm::ivec2 &posE, const glm::ivec2 &sizeE) const
{
	int min_xM, min_yM, max_xM, max_yM;
	int min_xE, min_yE, max_xE, max_yE;
	int sizeM_x = sizeM.x - 5;

	glm::ivec2 center_M, center_E;

	min_xM = posM.x + 5;
	max_xM = (posM.x + sizeM_x - 1);
	min_yM = posM.y;
	max_yM = posM.y + sizeM.y;

	min_xE = posE.x;
	max_xE = posE.x + sizeE.x;

	min_yE = posE.y - 1;
	max_yE = posE.y + sizeE.y + 1;


	if ((min_xM < max_xE) && (min_xE < max_xM) && (min_yM < max_yE) && (min_yE < max_yM)) {

		center_M = glm::vec2(posM.x + sizeM_x / 2, posM.y + sizeM.y / 2);
		center_E = glm::vec2(posE.x + sizeE.x / 2, posE.y + sizeE.y / 2);

		glm::vec2 direction = center_E - center_M;
		float module = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		glm::vec2 unitary_direction = glm::vec2(direction.x / module, direction.y / module);
		float angle = std::atan2(unitary_direction.y, unitary_direction.x);
		float angle_degrees = angle * 180.0f / M_PI;

		if (angle_degrees < 0.f)
			return 0; // DOWN
	}

	return -1;
}

pair<int,float> TileMap::collisionMarioEnemy(const glm::ivec2 &posM, const glm::ivec2 &sizeM, const glm::ivec2 &posE, const glm::ivec2 &sizeE) const
{
	int min_xM, min_yM, max_xM, max_yM;
	int min_xE, min_yE, max_xE, max_yE;
	int sizeM_x = sizeM.x - 5;
	int sizeE_y;
	if (sizeE.y == 64)
		sizeE_y = sizeE.y + 32;
	else
		sizeE_y = sizeE.y;

	glm::ivec2 center_M, center_E;

	min_xM = posM.x + 5;
	max_xM = (posM.x + sizeM_x - 1);
	min_yM = posM.y;
	max_yM = posM.y + sizeM.y;

	min_xE = posE.x + 2;
	max_xE = posE.x + sizeE.x - 2;

	if (sizeE.y == 64)
		min_yE = posE.y + 32;
	else
		min_yE = posE.y + 2;

	max_yE = posE.y + sizeE_y;


	if ((min_xM < max_xE) && (min_xE < max_xM) && (min_yM < max_yE) && (min_yE < max_yM)) {

		center_M = glm::vec2(posM.x + sizeM_x / 2, posM.y + sizeM.y / 2);
		center_E = glm::vec2(posE.x + sizeE.x / 2, posE.y + sizeE_y / 2);

		glm::vec2 direction = center_E - center_M;
		float module = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		glm::vec2 unitary_direction = glm::vec2(direction.x / module, direction.y / module);
		float angle = std::atan2(unitary_direction.y, unitary_direction.x);
		float angle_degrees = angle * 180.0f / M_PI;
		
		if (angle_degrees >= 45.f && angle_degrees <= 135.f)
			return make_pair(1,angle_degrees); // UP
		else if (angle_degrees < 0.f)
			return make_pair(0,angle_degrees); // DOWN
		else if (angle_degrees >= 0.f && angle_degrees < 45.f)
			return make_pair(2,angle_degrees); // LEFT
		else if (angle_degrees > 135.f && angle_degrees <= 180.f)
			return make_pair(3,angle_degrees); // RIGHT

	}

	return make_pair(-1,NULL);
}

int* TileMap::getMap()
{
	return map;
}

void TileMap::modify_position(int j, int i)
{
	map[(j / tileSize) * mapSize.x + (i / tileSize)] = 0;
}
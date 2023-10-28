#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
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
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
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

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, int *posX, bool margin, int center) const
{
	int x, y0, y1;
	
	x = (pos.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 3) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if ( ( ncol.find(map[y*mapSize.x + x]) == ncol.end() ) || margin)
		{
			if (margin) {
				*posX = center - 256;
				return true;
			}

			else if (*posX - tileSize * x - size.x <= 5 )
			{
				*posX = tileSize * x + size.x;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, int *posX) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 3) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if (ncol.find(map[y*mapSize.x + x]) == ncol.end())
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

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if(ncol.find(map[y*mapSize.x + x]) == ncol.end())
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

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (ncol.find(map[y*mapSize.x + x]) == ncol.end())
		{
			if (*posY - tileSize * y - size.y <= 5)
			{
				*posY = tileSize * y + 32;
				return true;
			}
		}
	}

	return false;
}

int TileMap::collisionMarioEnemy(const glm::ivec2 &posM, const glm::ivec2 &sizeM, const glm::ivec2 &posE, const glm::ivec2 &sizeE, int& way) const
{
	way = -1;// not usefoul value
	int min_xM, min_yM, max_xM, max_yM;
	int min_xE, min_yE, max_xE, max_yE;

	glm::ivec2 center_M, center_E;

	min_xM = posM.x;
	max_xM = posM.x + sizeM.x;
	min_yM = posM.y;
	max_yM = posM.y + sizeM.y;

	min_xE = posE.x;
	max_xE = posE.x + sizeE.x;
	min_yE = posE.y;
	max_yE = posE.y + sizeE.y;

	center_M = glm::ivec2(posM.x + sizeM.x / 2, posM.y + sizeM.y / 2);
	center_E = glm::ivec2(posE.x + sizeE.x / 2, posE.y + sizeE.y / 2);

	if ((min_xM < max_xE) && (min_xE < max_xM) && (min_yM < max_yE) && (min_yE < max_yM)) {
		glm::ivec2 diff = center_E - center_M;

		if (sizeM.y > sizeE.y)
			diff.y -= 16;
		else if (sizeM.y < sizeE.y)
			diff.y += 16;
		else if (sizeM.y == 64)
			diff.y -= 32;

		if (diff.y > 0) {
			return 1;
		}
		else {
			if (diff.x > 0) way = 1;// Mario hits from the enemy left side 
			else if (diff.x < 0) way = 0; // Mario hits from the enemy right side
			return 0;
		}

	}
	/*}
	else if(sizeM > sizeE)
	*/return -1;
}

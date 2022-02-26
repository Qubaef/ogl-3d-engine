#pragma once
#include <vector>

#include "Engine/Common/Vao.h"
#include "Engine/Common/Vbo.h"

#include "Terrain.h"


class RandomTerrain : public Terrain
{
	int sectorSizeX;
	int sectorSizeY;
	int sectorsNumberX;
	int sectorsNumberY;

	int pointsPerSectorLine;

	Vao mainVao;
	Vbo vertexBuffer;
	Vbo normalsBuffer;
	Vbo indicesBuffer;

	glm::vec3* vertexData;
	glm::vec3* normalsData;
	std::vector<int> indicesData;
	
public:
	RandomTerrain(int sectorSizeX, int sectorSizeY, int pointsPerSectorLine);

	void initialize() override;

	void update() override;

	void sendAndRender(Shader* shaderPtr) override;

	~RandomTerrain() override;
};

#pragma once
#include <vector>

#include "Terrain.h"

class FlatTerrain : Terrain
{
	Shader* shaderPtr;
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
	FlatTerrain(int sectorSizeX, int sectorSizeY, int sectorsNumberX, int sectorsNumberY, int pointsPerSectorLine, Shader* shaderPtr);

	void initialize() override;

	void update() override;

	void sendAndRender() override;

	Shader* getShader() override;

	~FlatTerrain() override;
};

#pragma once
#include <vector>

#include "Engine/Common/Vao.h"
#include "Engine/Common/Vbo.h"

#include "Terrain.h"

class FlatTerrain : Terrain
{
	int sectorSizeX;
	int sectorSizeY;

	int pointsPerSectorLine;

	Vao mainVao;
	Vbo vertexBuffer;
	Vbo normalsBuffer;
	Vbo indicesBuffer;

	glm::vec3* vertexData;
	glm::vec3* normalsData;
	std::vector<int> indicesData;

public:
	FlatTerrain(int sectorSizeX, int sectorSizeY, int pointsPerSectorLine);

	void initialize() override;

	void update() override;

	void sendAndRender(Shader* shaderPtr) override;

	~FlatTerrain() override;
};

#pragma once

#include <vector>

#include "Heightmap.h"
#include "Engine/CpuPipeline/Processable.h"
#include "Engine/Engine.h"
#include "QuadTerrainNode.h"
#include "Engine/Common/Vao.h"
#include "Engine/Common/Vbo.h"


class LodTerrain : public Processable
{
	Vao mainVao;
	Shader* shaderPtr = nullptr;

	// Handles to uniforms
	GLuint matMvpId;
	GLuint matMvId;
	GLuint matMId;
	GLuint matPId;

	GLuint nodePosId;
	GLuint nodeSizeId;
	GLuint terrainPosId;
	
	GLuint viewportId;
	GLuint terrainHeightId;
	GLuint terrainHeightOffsetId;
	GLuint terrainDensity;
	GLuint terrainSize;

	GLuint viewPosId;

	// Size of the heightmap
	SimplexNoiseWrapper noise;
	Heightmap* heightmap = nullptr;
	const int size = 10000;
	const int density = size;

	// Buffers common for all sectors
	Vbo vertexBuffer;
	vec3 vertexData[4];

	Vbo normalsBuffer;
	vec3 normalsData[4];

	Vbo indicesBuffer;
	int indicesData[4];

	// Terrain QuadTree
	QuadTerrainNode* terrainQuadTree = nullptr;

public:
	LodTerrain(Engine* enginePtr);

	void preprocess() override;

	void process() override;

	void render() override;

	~LodTerrain() override = default;
};

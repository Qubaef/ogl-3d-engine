#pragma once

#include "Heightmap.h"
#include "Engine/CpuPipeline/IProcessable.h"
#include "Engine/Engine.h"
#include "QuadTerrainNode.h"
#include "Engine/Common/Vao.h"
#include "Engine/Common/Vbo.h"

#include "Engine/MessageBus/IMessanger.h"


class SingleMeshLodTerrain : public IProcessable, public IMessanger
{
	Vao vao_main;
	Shader* shaderPtr = nullptr;

	// Handles to uniforms
	GLuint uniformId_matMvp;
	GLuint uniformId_matMv;
	GLuint uniformId_matM;
	GLuint uniformId_matP;

	GLuint uniformId_nodePos;
	GLuint uniformId_nodeSize;

	GLuint uniformId_viewport;
	GLuint uniformId_terrainHeight;
	GLuint uniformId_terrainDensity;
	GLuint uniformId_terrainSize;
	GLuint uniformId_terrainOffset;

	GLuint uniformId_viewPos;

	// Size of the heightmap
	SimplexNoiseWrapper noise;
	Heightmap* heightmap = nullptr;
	const int size;
	const int density;
	const int minLodPatchSize;
	vec3 terrainOffset = vec3(0,0,0);

	// Buffers common for all sectors
	Vbo vbo_vertex;
	vec3 vertexData[4];

	Vbo vbo_normals;
	vec3 normalsData[4];

	Vbo vbo_indices;
	int indicesData[4];

	// Terrain QuadTree
	QuadTerrainNode* terrainQuadTree = nullptr;

public:
	SingleMeshLodTerrain(Engine* enginePtr, int size, int density, int minLodPatchSize);

	void preprocess() override;

	void process() override;

	void render() override;

	~SingleMeshLodTerrain() override = default;
};

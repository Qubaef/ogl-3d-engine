#pragma once
#include "Engine/CpuPipeline/Processable.h"
#include "Terrain.h"
#include "Engine/Engine.h"

class TerrainManager : public Processable
{
	// Shader used for TerrainManager
	Shader* shaderPtr;

	// Terrain specific object, which provides operations on terrain mesh 
	Terrain* terrainPtr;

	// Handles to view matrices ids
	GLuint MvpMatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;

	void terrainInit(int x, int y, int pointsPerLine);

public:
	enum TERRAIN_TYPE
	{
		FLAT,
		RANDOM,
		WATER,
		SIMPLEX
	} terrainType;

	int x = 500;
	int y = 500;
	int pointsPerLine = 501;

	TerrainManager(Engine* enginePtr, TERRAIN_TYPE terrainType);

	void preprocess() override;

	void process() override;

	void render() override;

	~TerrainManager() override = default;
};

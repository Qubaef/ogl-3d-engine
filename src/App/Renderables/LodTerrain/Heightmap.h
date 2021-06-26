#pragma once

#include "App/Renderables/Terrain/SimplexNoiseWrapper.h"

class Engine;

class Heightmap
{
	unsigned size;
	unsigned density;

	const char* filename = "Heightmap.tga";
public:
	unsigned textureId;

	
	Heightmap(int size, int density);

	Heightmap(const char* filename);

	void update();
	
	void update(SimplexNoiseWrapper noise);

	// Generate texture with compute shader
	void updateGpu(Engine* enginePtr);

	void saveToFile(float* data);

	void erode(int droplets);

	void generateImage();

	~Heightmap();
};

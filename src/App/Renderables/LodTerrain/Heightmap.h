#pragma once

#include "App/Renderables/Terrain/SimplexNoiseWrapper.h"

class Engine;

class Heightmap
{
	const unsigned workgroupDiv = 25;	// Divisor also specified in compute shader
	const unsigned size;
	const unsigned density;

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

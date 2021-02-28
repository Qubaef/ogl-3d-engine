#pragma once
#include "RenderedEntityType.h"
#include "Terrain.h"

class TerrainType : RenderedEntityType
{
	// Terrain specific object, which provides operations on terrain mesh 
	Terrain* terrain;

public:
	TerrainType();
	
	void processPerFrame() override;
	void renderPerFrame() override;
	void sendGpuData() override;
};

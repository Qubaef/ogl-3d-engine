#pragma once

#include "EntityType.h"

class RenderedEntityType : public EntityType
{
protected:
	
public:
	// Render entity 
	virtual void renderPerFrame() = 0;

	// Send data to GPU
	virtual void sendGpuData() = 0;
};

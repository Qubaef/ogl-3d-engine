﻿#pragma once

#include "Engine/Components/Shaders/Shader.h"

class Terrain
{
public:
	
	// Each Terrain should provide it's own implementation of initialize method
	//  * initialize will be called during Engine preparation phase
	//  * it should prepare data structures and ogl buffers (glBufferData() to allocate), start used threads, load and set all obligatory data
	//  * it's execution time will effect engine loading time, so it is not required to be ultra optimized
	virtual void initialize() = 0;

	// Each Terrain should provide it's own implementation of process method (it can be empty)
	//  * process will be called every frame during runtime
	//  * it should process data basing on chosen conditions (it should not send the data to buffers yet)
	//  * it's execution time will effect engine runtime, so it is required to be optimized
	virtual void update() = 0;

	// Each Terrain should provide it's own implementation of render method (it can be empty)
	//  * sendData will be called only after it was registered as queued event in process method
	//  * it should send updated data to Gpu buffers used by the shader (glBufferSubData() to send the data)
	//  * it's execution time will effect engine runtime, so it is required to be optimized
	//  * difference between process and render is crucial, as process gets called in worker thread, which cannot
	//	  send or render on gpu
	virtual void sendAndRender(Shader* shaderPtr) = 0;

	// Destructor must be defined
	virtual ~Terrain() = default;
};

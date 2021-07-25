#pragma once
#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "../Shaders/Shader.h"

class Engine;
class SafeTaskQueue;

class Renderable
{
protected:
	Engine* enginePtr;

	// Handles to view matrices ids
	GLuint MvpMatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;

public:
	Renderable(Engine* enginePtr);

	// Process Processable behaviour for one frame
	virtual void processPerFrame();
	
	// Render entity 
	virtual void renderPerFrame() = 0;

	// Send data to GPU
	// virtual void sendGpuData() = 0;

	// Return shader object used by the entity
	virtual Shader& getShader() = 0;
};

#endif
#pragma once

#include "Engine/CpuPipeline/IProcessable.h"
#include "Engine/Engine.h"
#include "Engine/Common/Vao.h"
#include "Engine/Common/Vbo.h"

class Framebuffer : public IProcessable
{
	Shader* shaderPtr = nullptr;

	Vao mainVao;
	Vbo mainVbo;
	unsigned Fbo;					// Framebuffer object
	unsigned texColorFrameBuffer;	// Texture of framebuffer content object
	unsigned texDepthFrameBuffer;	// Texture of framebuffer content object

	float focusPoint = 15;
	float focusScale = 1.58;

	static float framebufferVertices[24];

public:
	Framebuffer(Engine* enginePtr);

	void preprocess() override;

	void process() override;

	void render() override;

	~Framebuffer() override = default;
};

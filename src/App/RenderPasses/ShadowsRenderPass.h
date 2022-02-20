#pragma once

#include "Engine/Components/MessageBus/IMessanger.h"
#include "Engine/Components/RenderPass/RenderPass.h"
#include "Renderer/OpenGL/Objects/GLUbo.h"

class DepthRenderPass;

//
// Shadows RenderPass uses DEFAULT DisplayMode to render all the components in regular manner
// Data is rendered to the screen space
// Shader requires GL_TEXTURE_2D_ARRAY from depth pass as an input
//

class ShadowsRenderPass : public RenderPass, public IMessanger
{
public:
	ShadowsRenderPass(Engine& engine, DepthRenderPass& depthRenderPass);

	void preRender() override;
	void postRender() override;

private:
	DepthRenderPass& depthRenderPass;

	float shadowStrength = 0.65f;
	bool shadowsCascadesDebug = false;
	GLUbo shadowsInfo;
};

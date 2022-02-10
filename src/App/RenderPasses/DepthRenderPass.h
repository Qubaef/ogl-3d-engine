#pragma once
#include "Engine/Components/RenderPass/RenderPass.h"

//
// Depth RenderPass uses DEPTH DisplayMode to render only the depth of all processables
// Data is rendered to frame-buffer's textures, which is then passed to ShadowsRenderPass
//
// Depth textures are used to generate cascaded shadows
//

class DepthRenderPass : public RenderPass
{
public:
	DepthRenderPass(Engine& engine);

	void preRender() override;

	void postRender() override;
};

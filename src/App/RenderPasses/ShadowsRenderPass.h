#pragma once

#include "Engine/Components/RenderPass/RenderPass.h"
#include "Renderer/OpenGl/Objects/Ubo.h"

//
// Shadows RenderPass uses DEFAULT DisplayMode to render all the components in regular manner
// Data is rendered to the screen space
// Shader requires GL_TEXTURE_2D_ARRAY from depth pass as an input
//

class ShadowsRenderPass : public RenderPass
{
public:
	ShadowsRenderPass(Engine& engine);

	// void setDepthTextures();

	void preRender() override;

	void postRender() override;

private:
	// GlTexture2dArray lightDepthMapsArray;
	Ubo lightMatricesUbo;
};

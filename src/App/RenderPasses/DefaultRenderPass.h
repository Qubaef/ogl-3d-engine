#pragma once

#include "Engine/Components/RenderPass/RenderPass.h"

class DefaultRenderPass : public RenderPass
{
public:
	DefaultRenderPass(Engine& engine);

	void preRender() override;
	void postRender() override;
};

#include "DefaultRenderPass.h"

#include "Engine/Engine.h"
#include "Engine/Components/Shaders/ShaderGlobalData.h"

DefaultRenderPass::DefaultRenderPass(Engine& engine) :
	RenderPass(engine)
{
}

void DefaultRenderPass::preRender()
{
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get shaderGlobalData and set display type to default
	ShaderGlobalData& shaderGlobalData = engine.getShaderGlobalData();
	shaderGlobalData.data.displayMode = ShaderGlobalData::DisplayMode::DEFAULT;

	shaderGlobalData.updateToGpu();
	shaderGlobalData.bind(engine.props.consts.GLOBAL_DATA_BIND_ID);
}

void DefaultRenderPass::postRender()
{
}

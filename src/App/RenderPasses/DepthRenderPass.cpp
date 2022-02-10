#include "DepthRenderPass.h"

#include "Engine/Engine.h"
#include "Engine/Components/Shaders/ShaderGlobalData.h"

DepthRenderPass::DepthRenderPass(Engine& engine) :
	RenderPass(engine)
{
}

void DepthRenderPass::preRender()
{
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get shaderGlobalData and set display type to default
	ShaderGlobalData& shaderGlobalData = engine.getShaderGlobalData();

	shaderGlobalData.set_displayMode(static_cast<int>(ShaderGlobalData::DisplayMode::DEFAULT));

	shaderGlobalData.set_viewPos(glm::vec4(1));
	shaderGlobalData.set_viewDir(glm::vec4(2));
	shaderGlobalData.set_nearPlane(3);
	shaderGlobalData.set_farPlane(4);

	shaderGlobalData.set_MVP(glm::mat4(5));
	shaderGlobalData.set_M(glm::mat4(6));
	shaderGlobalData.set_M_inv(glm::mat4(7));
	shaderGlobalData.set_V(glm::mat4(8));
	shaderGlobalData.set_P(glm::mat4(9));

	shaderGlobalData.set_dirLightAmbient(glm::vec4(10));
	shaderGlobalData.set_dirLightDiffuse(glm::vec4(11));
	shaderGlobalData.set_dirLightSpecular(glm::vec4(12));
	shaderGlobalData.set_dirLightDirection(glm::vec4(13));

	shaderGlobalData.bind(0);
}

void DepthRenderPass::postRender()
{
	// engine.postRenderPassData(this, TargetRenderPass::NEXT, data);
}

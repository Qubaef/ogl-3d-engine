#include "ShadowsRenderPass.h"

#include "DepthRenderPass.h"
#include "App/Renderables/GuiEntityManager/EntityProperties/BoolPropertyTrueFalseSwitch.h"
#include "App/Renderables/GuiEntityManager/EntityProperties/FloatPropertyContinuousModifier.h"
#include "App/Renderables/GuiEntityManager/Messages/RegisterEntityMessage.h"
#include "App/Renderables/GuiEntityManager/Messages/RegisterPropertyMessage.h"
#include "Engine/Engine.h"
#include "Engine/Components/Shaders/ShaderGlobalData.h"


ShadowsRenderPass::ShadowsRenderPass(Engine& engine, DepthRenderPass& depthRenderPass) :
	RenderPass(engine),
	IMessanger(&engine.getMessageBus(), "ShadowsRenderPass"),
	depthRenderPass(depthRenderPass)
{
	//
	// Setup shadows info
	//
	shadowsInfo.init(sizeof(glm::vec4));

	//
	// Send messages to entity manager
	//
	sendMessage(new RegisterEntityMessage(""), "EntityManager");

	sendMessage(new RegisterPropertyMessage("ShadowsRenderPass",
		new FloatPropertyContinuousModifier("shadowStrength", 0, 1, shadowStrength)),
		"EntityManager");

	sendMessage(new RegisterPropertyMessage("ShadowsRenderPass",
		new BoolPropertyTrueFalseSwitch("Cascades debug view", shadowsCascadesDebug)),
		"EntityManager");
}

void ShadowsRenderPass::preRender()
{
	//
	// Setup render
	//
	glViewport(0, 0,
		engine.props.consts.windowWidth,
		engine.props.consts.windowHeight
	);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Enable default engine's face culling
	// TODO: extern it to the engine so that it is available globally
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	//
	//  Setup shaderGlobalData and send it to GPU
	//
	
	// Get shaderGlobalData and set display type to default
	ShaderGlobalData& shaderGlobalData = engine.getShaderGlobalData();
	shaderGlobalData.data.displayMode = ShaderGlobalData::DisplayMode::DEFAULT;
	if (shadowsCascadesDebug)
		shaderGlobalData.data.displayMode |= ShaderGlobalData::DisplayMode::CASCADES_DEBUG;

	shaderGlobalData.data.viewPos = glm::vec4(engine.getCamera()->getPosition(), 1.0);
	shaderGlobalData.data.viewDir = glm::vec4(engine.getCamera()->getDirection(), 1.0);
	shaderGlobalData.data.nearPlane = engine.props.consts.cameraNearClipping;
	shaderGlobalData.data.farPlane = engine.props.consts.cameraFarClipping;

	shaderGlobalData.data.MVP = *(engine.getCamera()->getMVPMatrix());
	shaderGlobalData.data.M = *(engine.getCamera()->getModelMatrix());
	shaderGlobalData.data.V = *(engine.getCamera()->getViewMatrix());
	shaderGlobalData.data.P = *(engine.getCamera()->getProjectionMatrix());

	shaderGlobalData.data.dirLightAmbient = glm::vec4(engine.getShaderManager().getDirectionalLight().getColorAmbient(), 1.0);
	shaderGlobalData.data.dirLightDiffuse = glm::vec4(engine.getShaderManager().getDirectionalLight().getColorDiffuse(), 1.0);
	shaderGlobalData.data.dirLightSpecular = glm::vec4(engine.getShaderManager().getDirectionalLight().getColorSpecular(), 1.0);
	shaderGlobalData.data.dirLightDirection = glm::vec4(engine.getShaderManager().getDirectionalLight().getDirectionVal(), 1.0);

	shaderGlobalData.updateToGpu();
	shaderGlobalData.bind(engine.props.consts.GLOBAL_DATA_BIND_ID);

	//
	// Setup textures and Ubos
	//
	depthRenderPass.getLightDepthTexturesArray().bind(GL_TEXTURE0);
	depthRenderPass.getLightMatricesUbo().bind(engine.props.consts.GLOBAL_DATA_BIND_ID + 1);
	depthRenderPass.getCascadesUbo().bind(engine.props.consts.GLOBAL_DATA_BIND_ID + 2);

	shadowsInfo.setData(0, sizeof(float), &shadowStrength);
	shadowsInfo.bind(engine.props.consts.GLOBAL_DATA_BIND_ID + 3);
}

void ShadowsRenderPass::postRender()
{
}

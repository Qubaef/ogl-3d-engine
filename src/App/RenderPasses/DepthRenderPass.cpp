#include "DepthRenderPass.h"

#include "App/Renderables/GuiEntityManager/EntityProperties/FloatPropertyContinuousModifier.h"
#include "App/Renderables/GuiEntityManager/EntityProperties/IntPropertyContinuousSliderModifier.h"
#include "App/Renderables/GuiEntityManager/EntityProperties/LambdaProperty.h"
#include "App/Renderables/GuiEntityManager/EntityProperties/TexturePropertyWatcher.h"
#include "App/Renderables/GuiEntityManager/Messages/RegisterEntityMessage.h"
#include "App/Renderables/GuiEntityManager/Messages/RegisterPropertyMessage.h"

#include "Engine/Engine.h"
#include "Engine/Components/Shaders/ShaderGlobalData.h"
#include "Renderer/OpenGL/Utils.h"
#include "Renderer/OpenGL/Validation.h"

using namespace glm;

DepthRenderPass::DepthRenderPass(Engine& engine) :
	RenderPass(engine),
	IMessanger(&engine.getMessageBus(), "DepthRenderPass")
{
	//
	// Generate cascades boundaries
	//
	calculateCascades();

	//
	// Initialize lightDepthTexturesArray
	//
	constexpr float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	constexpr GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };

	lightDepthTexturesArray.init(
		GL_DEPTH_COMPONENT32F,
		DEPTH_MAP_RES,
		DEPTH_MAP_RES,
		SHADOW_CASCADES_COUNT + 1,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		GL_NEAREST,
		GL_NEAREST,
		GL_CLAMP_TO_BORDER
	);
	lightDepthTexturesArray.setBorder(borderColor);

	//
	// Initialize depthFramebuffer
	//
	depthFramebuffer.init();
	depthFramebuffer.attachTexture(GL_DEPTH_ATTACHMENT, lightDepthTexturesArray.getId());
	depthFramebuffer.setDrawBuffer(GL_NONE);
	depthFramebuffer.setReadBuffer(GL_NONE);
	depthFramebuffer.validate();

	//
	// Initialize cascadeDebugLayerTexture
	//
	cascadeDebugLayerTexture.init(
		GL_DEPTH_COMPONENT32F,
		DEPTH_MAP_RES,
		DEPTH_MAP_RES,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		GL_NEAREST,
		GL_NEAREST,
		GL_CLAMP_TO_BORDER
	);
	cascadeDebugLayerTexture.setBorder(borderColor);
	cascadeDebugLayerTexture.setSwizzle(swizzleMask);

	//
	// Initialize lightMatricesUbo
	//
	lightMatricesUbo.init(MAX_LIGHT_MATRICES * sizeof(mat4x4));

	//
	// Send messages to entity manager
	//
	sendMessage(new RegisterEntityMessage(""), "EntityManager");

	sendMessage(new RegisterPropertyMessage("DepthRenderPass",
		new FloatPropertyContinuousModifier("zMultiplier", 0, 15, zMultiplier)),
		"EntityManager");

	sendMessage(new RegisterPropertyMessage("DepthRenderPass",
		new IntPropertyContinuousSliderModifier(
			"Cascade layer id",
			0,
			shadowCascadeLevelsList.size() - 1,
			debugLayerId)
	),
		"EntityManager");

	sendMessage(
		new RegisterPropertyMessage("DepthRenderPass",
			new TexturePropertyWatcher(
				"shadowMapLayerTexture",
				cascadeDebugLayerTexture.getId(),
				DEPTH_MAP_RES,
				DEPTH_MAP_RES,
				engine.props.consts.windowWidth * 0.20f,
				engine.props.consts.windowWidth * 0.20f
			)
		),
		"EntityManager"
	);

	sendMessage(
		new RegisterPropertyMessage("DepthRenderPass",
			new LambdaProperty(
				"",
				[&]() {
					if (ImGui::TreeNode("Advanced properties:"))
					{
						if (debugLayerId == 0)
						{
							float zeroBound = 0;
							ImGui::DragFloat("Lower bound: base", &zeroBound,
								0.f, 0, 0, "%f",
								ImGuiSliderFlags_AlwaysClamp
							);

							ImGui::DragFloat(
								"Upper bound: 0",
								&shadowCascadeLevelsList[0],
								engine.props.consts.cameraFarClipping / 10000,
								0, engine.props.consts.cameraFarClipping,
								"%f",
								ImGuiSliderFlags_AlwaysClamp
							);
						}
						else
						{
							ImGui::DragFloat(
								("Lower bound: " + std::to_string(debugLayerId - 1)).c_str(),
								&shadowCascadeLevelsList[debugLayerId - 1],
								engine.props.consts.cameraFarClipping / 10000,
								0, engine.props.consts.cameraFarClipping,
								"%f",
								ImGuiSliderFlags_AlwaysClamp
							);

							ImGui::DragFloat(
								("Upper bound: " + std::to_string(debugLayerId)).c_str(),
								&shadowCascadeLevelsList[debugLayerId],
								engine.props.consts.cameraFarClipping / 10000,
								0, engine.props.consts.cameraFarClipping,
								"%f",
								ImGuiSliderFlags_AlwaysClamp
							);
						}

						ImGui::DragFloat(
							("shadowCascadeBias: " + std::to_string(debugLayerId)).c_str(),
							&shadowCascadeBiasesList[debugLayerId],
							0.001f, 0, 1, "%f"
						);

						ImGui::TreePop();
					}
				}
			)
		),
		"EntityManager"
	);
}

void DepthRenderPass::preRender()
{
	//
	// Setup render
	//
	glViewport(0, 0, DEPTH_MAP_RES, DEPTH_MAP_RES);
	depthFramebuffer.bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Reverse face culling to minimize peter-panning
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	updateCascades();

	//
	//  Setup shaderGlobalData and send it to GPU
	//

	// Get shaderGlobalData and set display type to depth
	ShaderGlobalData& shaderGlobalData = engine.getShaderGlobalData();
	shaderGlobalData.data.displayMode = ShaderGlobalData::DisplayMode::DEPTH;

	shaderGlobalData.updateToGpu();
	shaderGlobalData.bind(engine.props.consts.GLOBAL_DATA_BIND_ID);

	//
	// Calculate and setup light matrices
	//
	setupLightMatrices();
	lightMatricesUbo.bind(engine.props.consts.GLOBAL_DATA_BIND_ID + 1);
}

void DepthRenderPass::postRender()
{
	// Disable rendering to framebuffer
	GLFramebuffer::unbind();

	// Copy current shadow map layer to 
	copyTextureArrayLayerToGL_TEXTURE_2D(
		cascadeDebugLayerTexture.getId(),
		lightDepthTexturesArray.getId(),
		debugLayerId, DEPTH_MAP_RES, DEPTH_MAP_RES
	);
}

GLTexture2dArray DepthRenderPass::getLightDepthTexturesArray() const
{
	return lightDepthTexturesArray;
}

GLUbo DepthRenderPass::getLightMatricesUbo() const
{
	return lightMatricesUbo;
}

GLUbo DepthRenderPass::getCascadesUbo() const
{
	return cascadesUbo;
}

void DepthRenderPass::calculateCascades()
{
	//
	// Generate shadow cascade levels boundaries
	//
	int base = 40;
	int prev = 0;

	shadowCascadeLevelsList.clear();

	for (int i = 0; i < SHADOW_CASCADES_COUNT; i++)
	{
		prev = prev + base * pow(2, i);
		shadowCascadeLevelsList.push_back(prev);
	}

	//
	// Generate shadow cascades biases
	//
	shadowCascadeBiasesList.clear();

	shadowCascadeBiasesList.push_back(0.0050f);
	shadowCascadeBiasesList.push_back(0.0352f);
	shadowCascadeBiasesList.push_back(0.0767f);
	shadowCascadeBiasesList.push_back(0.1650f);
	shadowCascadeBiasesList.push_back(0.3280f);

	//
	// Initialize cascadesUBO and set it's data
	//
	cascadesUbo.init(MAX_LIGHT_MATRICES * 2 * sizeof(vec4) + sizeof(vec4));

	// Cascade boundaries
	int baseOffset = 0;
	for (int i = 0; i < shadowCascadeLevelsList.size(); i++)
	{
		cascadesUbo.setData(
			baseOffset + i * sizeof(vec4),
			sizeof(float),
			&shadowCascadeLevelsList[i]
		);
	}

	// Cascade boundaries
	baseOffset = MAX_LIGHT_MATRICES * sizeof(vec4);
	for (int i = 0; i < shadowCascadeBiasesList.size(); i++)
	{
		cascadesUbo.setData(
			baseOffset + i * sizeof(vec4),
			sizeof(float),
			&shadowCascadeBiasesList[i]
		);
	}

	// Cascade count
	baseOffset = 2 * MAX_LIGHT_MATRICES * sizeof(vec4);
	int cascadeCount = shadowCascadeLevelsList.size();
	cascadesUbo.setData(
		baseOffset,
		sizeof(int),
		&cascadeCount
	);
}

void DepthRenderPass::updateCascades()
{
	// Cascade boundaries
	int baseOffset = 0;
	for (int i = 0; i < shadowCascadeLevelsList.size(); i++)
	{
		cascadesUbo.setData(
			baseOffset + i * sizeof(vec4),
			sizeof(float),
			&shadowCascadeLevelsList[i]
		);
	}

	// Cascade boundaries
	baseOffset = MAX_LIGHT_MATRICES * sizeof(vec4);
	for (int i = 0; i < shadowCascadeBiasesList.size(); i++)
	{
		cascadesUbo.setData(
			baseOffset + i * sizeof(vec4),
			sizeof(float),
			&shadowCascadeBiasesList[i]
		);
	}

	// Cascade count
	baseOffset = 2 * MAX_LIGHT_MATRICES * sizeof(vec4);
	int cascadeCount = shadowCascadeLevelsList.size();
	cascadesUbo.setData(
		baseOffset,
		sizeof(int),
		&cascadeCount
	);
}

std::vector<vec4> DepthRenderPass::getFrustumCornersWorldSpace(const mat4& proj, const mat4& view)
{
	const auto inv = inverse(proj * view);

	std::vector<vec4> frustumCorners;
	for (unsigned int x = 0; x < 2; ++x)
	{
		for (unsigned int y = 0; y < 2; ++y)
		{
			for (unsigned int z = 0; z < 2; ++z)
			{
				const vec4 pt =
					inv * vec4(
						2.0f * x - 1.0f,
						2.0f * y - 1.0f,
						2.0f * z - 1.0f,
						1.0f);
				frustumCorners.push_back(pt / pt.w);
			}
		}
	}

	return frustumCorners;
}

mat4 DepthRenderPass::calculateLightSpaceMatrix(const float nearPlane, const float farPlane)
{
	vec3 lightDir = engine.getShaderManager().getDirectionalLight().getDirectionVal();
	Camera* cameraPtr = engine.getCamera();

	mat4 proj = perspective(
		(float)engine.props.consts.cameraFov,
		(float)engine.props.consts.windowWidth / (float)engine.props.consts.windowHeight,
		nearPlane, farPlane
	);

	mat4 view = *cameraPtr->getViewMatrix();
	auto corners = getFrustumCornersWorldSpace(proj, view);

	vec3 targetPoint = vec3(0, 0, 0);
	for (const auto& v : corners)
	{
		targetPoint += vec3(v);
	}
	targetPoint /= corners.size();

	const auto lightView = lookAt(
		targetPoint + normalize(lightDir),
		targetPoint,
		vec3(0.0f, 1.0f, 0.0f)
	);

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::min();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::min();
	for (const auto& v : corners)
	{
		const auto trf = lightView * v;
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	// Tune this parameter according to the scene
	if (minZ < 0) {
		minZ *= zMultiplier;
	}
	else {
		minZ /= zMultiplier;
	}
	if (maxZ < 0) {
		maxZ /= zMultiplier;
	}
	else {
		maxZ *= zMultiplier;
	}

	const mat4 lightProjection = ortho(
		minX, maxX,
		minY, maxY,
		minZ, maxZ);

	return lightProjection * lightView;
}

std::vector<mat4> DepthRenderPass::calculateLightSpaceMatrices()
{
	float cameraNearPlane = engine.props.consts.cameraNearClipping;
	float cameraFarPlane = engine.props.consts.cameraFarClipping;
	std::vector<mat4> ret;

	for (int i = 0; i < SHADOW_CASCADES_COUNT + 1; i++)
	{
		if (i == 0)
		{
			ret.push_back(calculateLightSpaceMatrix(cameraNearPlane, shadowCascadeLevelsList[i]));
		}
		else if (i < shadowCascadeLevelsList.size())
		{
			ret.push_back(calculateLightSpaceMatrix(shadowCascadeLevelsList[i - 1], shadowCascadeLevelsList[i]));
		}
		else
		{
			ret.push_back(calculateLightSpaceMatrix(shadowCascadeLevelsList[i - 1], cameraFarPlane));
		}
	}

	return ret;
}

void DepthRenderPass::setupLightMatrices()
{
	const std::vector<glm::mat4> lightMatrices = calculateLightSpaceMatrices();

	for (size_t i = 0; i < lightMatrices.size(); ++i)
	{
		lightMatricesUbo.setData(i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), (void*)value_ptr(lightMatrices[i]));
	}
}

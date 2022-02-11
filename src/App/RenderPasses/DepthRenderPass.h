#pragma once
#include "Engine/Components/MessageBus/IMessanger.h"
#include "Engine/Components/RenderPass/RenderPass.h"
#include "Renderer/OpenGL/Objects/GLFramebuffer.h"
#include "Renderer/OpenGL/Objects/GLTexture2d.h"
#include "Renderer/OpenGL/Objects/GLTexture2dArray.h"
#include "Renderer/OpenGL/Objects/GLUbo.h"

//
// Depth RenderPass uses DEPTH DisplayMode to render only the depth of all processables
// Data is rendered to frame-buffer's textures, which is then passed to ShadowsRenderPass
//
// Depth textures are used to generate cascaded shadows
//

class DepthRenderPass : public RenderPass, public IMessanger
{
public:
	DepthRenderPass(Engine& engine);

	void preRender() override;
	void postRender() override;

	GLTexture2dArray getLightDepthTexturesArray() const;
	GLUbo getLightMatricesUbo() const;
	GLUbo getCascadesUbo() const;

private:
	// Const parameters
	const unsigned int DEPTH_MAP_RES = 4096;
	const unsigned int SHADOW_CASCADES_COUNT = 5;
	const unsigned int MAX_LIGHT_MATRICES = 16;

	// Modifiable parameters
	float zMultiplier = 3.9f;
	int debugLayerId = 0;

	// Internal components
	GLFramebuffer depthFramebuffer;
	GLTexture2dArray lightDepthTexturesArray;
	GLTexture2d cascadeDebugLayerTexture;
	GLUbo lightMatricesUbo;
	GLUbo cascadesUbo;
	std::vector<float> shadowCascadeLevelsList;
	std::vector<float> shadowCascadeBiasesList;

	// Internal methods
	void calculateCascades();

	std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
	glm::mat4 calculateLightSpaceMatrix(const float nearPlane, const float farPlane);
	std::vector<glm::mat4> calculateLightSpaceMatrices();
	void setupLightMatrices();
};

#pragma once

#include "Engine/CpuPipeline/IProcessable.h"
#include "Engine/Engine.h"
#include "Engine/Common/Vao.h"
#include "Engine/Common/Vbo.h"

class Interior : public IProcessable, public IMessanger
{
	unsigned shadowMapLayerTexture;
	bool debug = false;
	// float maxShadowBias = 0.02;
	float shadowStrength = 0.65f;
	std::vector<float> maxShadowCascadeBiasList;
	float tmp = 1.0f;

	float zMult = 3.9f;
	std::vector<float> shadowCascadeLevels;
	int layer = 0;
	unsigned int matricesUBO;

	// Shadows
	unsigned int depthFramebuffer;
	const unsigned int DEPTH_MAP_RES = 4096;

	std::vector<unsigned> shadowMapTextures;
	unsigned int lightDepthMaps;

    // Shader used for Interior
	Shader* shaderPtr;
	Shader* shadowMappingShaderPtr;
	Shader* framebufferShaderPtr;

	// Handles to view matrices ids
	GLuint MvpMatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;

    vec3 pos;

	// Interior
    float scaleX = 1000;
    float scaleZ = 1000;
	float wallHeight = 10;
    int meshDensity = 10;

	Vao mainVao;
	Vbo vertexBuffer;
	Vbo normalsBuffer;
	Vbo indicesBuffer;

	std::vector<glm::vec3> vertexData;
	std::vector<glm::vec3> normalsData;
	std::vector<int> indicesData;

	Vao boxesVao;
	Vbo vertexBuffer2;
	Vbo normalsBuffer2;
	Vbo indicesBuffer2;

	std::vector<glm::vec3> vertexData2;
	std::vector<glm::vec3> normalsData2;
	std::vector<int> indicesData2;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	std::vector<glm::mat4> getLightSpaceMatrices();
	std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);

	void generateBox(vec3 pos, int scale);

	void generateFloor();

	void generateWall(vec2 wall, vec3 direction);


	mat4 calculateLightSpaceMatrix(const float nearPlane, const float farPlane);


	void setupDepthShaderAndMatrices();
	void setupShaderAndMatrices();

	void renderScene();

	void renderQuad();

public:
	Interior(Engine* enginePtr);

	void preprocess() override;

	void process() override;

	void render() override;

	~Interior() override = default;
};

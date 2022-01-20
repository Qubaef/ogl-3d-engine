#pragma once

#include "Engine/CpuPipeline/IProcessable.h"
#include "Engine/Engine.h"
#include "Engine/Common/Vao.h"
#include "Engine/Common/Vbo.h"

class Interior : public IProcessable, public IMessanger
{
	float SHADOW_LIGHT_MULTIPLIER = 100;
	float orthoSize = 100;
	float nearPane = 1.f;
	float fearPane = 200.f;

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

	// Shadows
	unsigned int depthMapFBO;
	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	unsigned int depthMap;

	void generateBox(vec3 pos, int scale);

	void generateFloor();

	void generateWall(vec2 wall, vec3 direction);


	mat4 calculateLightSpaceMatrix();


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

#pragma once

#include "Engine/CpuPipeline/IProcessable.h"
#include "Engine/Engine.h"
#include "Engine/Common/Vao.h"
#include "Engine/Common/Vbo.h"

class InteriorNoShading : public IProcessable, public IMessanger
{
public:
	InteriorNoShading(Engine& engine);

	void preprocess() override;
	void process() override;
	void render() override;

	~InteriorNoShading() override = default;

private:
	std::vector<float> maxShadowCascadeBiasList;

    // Shader used for Interior
	Shader* depthShader;
	Shader* lightingShadowsShader;

	// Handles to view matrices ids
	GLuint MvpMatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;

	glm::vec3 pos;

	// Interior
    const float scaleX = 1000;
    const float scaleZ = 1000;
	const float wallHeight = 10;
    const int meshDensity = 10;

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

	void generateBox(glm::vec3 pos, int scale);
	void generateFloor();
	void generateWall(glm::vec2 wall, glm::vec3 direction);

	void setupDepthRender();
	void setupDefaultRender();

	void renderScene();
};

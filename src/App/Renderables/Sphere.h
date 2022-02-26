#pragma once

#include "Engine/CpuPipeline/IProcessable.h"
#include "Engine/Engine.h"
#include "Engine/Common/Vao.h"
#include "Engine/Common/Vbo.h"

class Sphere : public IProcessable
{
	// Shader used for Sphere
	Shader* depthShader;
	Shader* lightingShadowsShader;

	unsigned xSegments = 30;
	unsigned ySegments = 30;
	unsigned radius = 1;
	glm::vec3 pos;

	Vao mainVao;
	Vbo vertexBuffer;
	Vbo normalsBuffer;
	Vbo indicesBuffer;

	std::vector<glm::vec3> vertexData;
	std::vector<glm::vec3> normalsData;
	std::vector<int> indicesData;

public:
	Sphere(Engine& engine, glm::vec3 pos = glm::vec3(0, 1, 0));

	void preprocess() override;

	void process() override;

	void setupDepthRender();
	void setupDefaultRender();
	void renderScene();
	void render() override;

	~Sphere() override = default;
};

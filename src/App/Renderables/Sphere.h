#pragma once

#include "Engine/CpuPipeline/Processable.h"
#include "Engine/Engine.h"
#include "Engine/Common/Vao.h"
#include "Engine/Common/Vbo.h"

class Sphere : public Processable
{
	// Shader used for TerrainManager
	Shader* shaderPtr;

	// Handles to view matrices ids
	GLuint MvpMatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;

	unsigned xSegments = 30;
	unsigned ySegments = 30;
	unsigned radius = 1;
	vec3 pos;

	Vao mainVao;
	Vbo vertexBuffer;
	Vbo normalsBuffer;
	Vbo indicesBuffer;

	std::vector<glm::vec3> vertexData;
	std::vector<glm::vec3> normalsData;
	std::vector<int> indicesData;

public:
	Sphere(Engine* enginePtr);

	void preprocess() override;

	void process() override;
	
	void render() override;

	~Sphere() override = default;
};

#pragma once

#include <string>
#include <vector>

#include "Engine/Common/Vao.h"
#include "Engine/Common/Vbo.h"
#include "Engine/CpuPipeline/IProcessable.h"
#include "Engine/Shaders/Shader.h"

using namespace glm;

class Skybox : public IProcessable
{
	Shader* shaderPtr;
	GLuint skyboxTextureID;
	static float skyboxVertices[108];

	Vao mainVao;
	Vbo vertexBuffer;

	// Handles to view matrices ids
	GLuint ProjectionMatrixID;
	GLuint ViewMatrixID;

	std::vector<std::string> skyboxFilenames;
	int width;
	int height;
	int nrChannels;

public:
	Skybox(Engine* enginePtr);

	void preprocess() override;
	
	void process() override;

	void render() override;

	~Skybox() override = default;

};

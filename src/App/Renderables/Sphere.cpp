#include "Sphere.h"

using namespace glm;

Sphere::Sphere(Engine& engine, vec3 pos)
	: IProcessable(engine), pos(pos)
{
	depthShader = engine.getShaderByName("DepthShader");
	lightingShadowsShader = engine.getShaderByName("LightingShadowsShader");


	for (unsigned int y = 0; y <= ySegments; ++y)
	{
		for (unsigned int x = 0; x <= xSegments; ++x)
		{
			float xSegment = (float)x / (float)xSegments;
			float ySegment = (float)y / (float)ySegments;
			float xPos = radius * std::cos(xSegment * 2 * pi<float>()) * std::sin(ySegment * pi<float>()) + pos.x;
			float yPos = radius * std::cos(ySegment * pi<float>()) + pos.y;
			float zPos = radius * std::sin(xSegment * 2 * pi<float>()) * std::sin(ySegment * pi<float>()) + pos.z;

			vertexData.push_back(vec3(xPos, yPos, zPos));
			normalsData.push_back(vec3(xPos - pos.x, yPos - pos.y, zPos - pos.z));
		}
	}

	for (int y = 0; y < ySegments; ++y)
	{
		for (int x = 0; x < xSegments; ++x)
		{
			indicesData.push_back((y + 1) * (xSegments + 1) + x);
			indicesData.push_back(y * (xSegments + 1) + x);
			indicesData.push_back(y * (xSegments + 1) + x + 1);

			indicesData.push_back((y + 1) * (xSegments + 1) + x);
			indicesData.push_back(y * (xSegments + 1) + x + 1);
			indicesData.push_back((y + 1) * (xSegments + 1) + x + 1);
		}
	}

	// Attributes setup and global VAO creation

	// bind global VAO object
	glBindVertexArray(mainVao.id);

	// select indices VBO (to perform VBO indexing)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer.id);
	// bind indices VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(int), &indicesData[0], GL_STATIC_DRAW);

	//// 1st vertex shader input attribute - vertex data
	// select vertex VBO
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(vec3), &vertexData[0], GL_STATIC_DRAW);
	// redirect buffer to input of the shader
	glVertexAttribPointer(
		0,						// location in shader
		3,						// size
		GL_FLOAT,				// type
		GL_FALSE,				// normalized
		0,						// stride (space between following fragments)
		(void*)0				// array buffer offset
	);
	// enable attribute '0'
	glEnableVertexAttribArray(0);

	// 2nd vertex shader input attribute - normals
	// select normals VBO
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer.id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, normalsData.size() * sizeof(vec3), &normalsData[0], GL_STATIC_DRAW);
	// redirect buffer to input of the shader
	glVertexAttribPointer(
		1,                      // location in shader
		3,                      // size
		GL_FLOAT,               // type
		GL_FALSE,               // normalized
		0,                      // stride (space between following fragments)
		(void*)0                // array buffer offset
	);
	// enable attribute '1'
	glEnableVertexAttribArray(1);
}

void Sphere::preprocess()
{
}

void Sphere::process()
{
}

void Sphere::setupDepthRender()
{
	// Render scene from light's point of view
	depthShader->use();
}

void Sphere::setupDefaultRender()
{
	// Render scene from camera's point of view
	lightingShadowsShader->use();

	lightingShadowsShader->setVec3("material.ambient", vec3(0.0215, 0.1745, 0.0215));
	lightingShadowsShader->setVec3("material.diffuse", vec3(0.07568, 0.61424, 0.07568));
	lightingShadowsShader->setVec3("material.specular", vec3(0.633, 0.727811, 0.633));
	lightingShadowsShader->setFloat("material.shininess", 2);
}

void Sphere::renderScene()
{
	// bind global VAO object
	glBindVertexArray(mainVao.id);

	glCullFace(GL_BACK);

	// Draw
	glDrawElements(
		GL_TRIANGLES,			// mode
		indicesData.size(),		// count
		GL_UNSIGNED_INT,		// type
		0				// element array buffer offset
	);

	glCullFace(GL_FRONT);
}

void Sphere::render()
{
	if ((engine.getShaderGlobalData().data.displayMode & ShaderGlobalData::DisplayMode::DEPTH) > 0)
	{
		setupDepthRender();
	}
	else if ((engine.getShaderGlobalData().data.displayMode & ShaderGlobalData::DisplayMode::DEFAULT) > 0)
	{
		setupDefaultRender();
	}

	renderScene();
}

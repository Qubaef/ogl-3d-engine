#include "WaterTerrain.h"

using namespace glm;

WaterTerrain::WaterTerrain(int sectorSizeX, int sectorSizeY, int sectorsNumberX, int sectorsNumberY, int pointsPerSectorLine, Shader* shaderPtr)
	: shaderPtr(shaderPtr),
	sectorSizeX(sectorSizeX),
	sectorSizeY(sectorSizeY),
	sectorsNumberX(sectorsNumberX),
	sectorsNumberY(sectorsNumberY),
	pointsPerSectorLine(pointsPerSectorLine)
{
	ZoneScoped;

	this->shaderPtr->use();
	this->shaderPtr->setVec3("material.ambient", vec3(0.f, 0.41f, 0.58f));
	this->shaderPtr->setVec3("material.diffuse", vec3(0.f, 0.61f, 0.78f));
	this->shaderPtr->setVec3("material.specular", vec3(0.6f, 0.6f, 0.6f));
	this->shaderPtr->setFloat("material.shininess", 128);

	// wave parameters (direction), steepness, wavelength
	wave1 = vec4(normalize(vec2(1, 1)), 0.25, 60);
	wave2 = vec4(normalize(vec2(1, 0.6)), 0.25, 31);
	wave3 = vec4(normalize(vec2(1, 1.3)), 0.25, 18);

	// Generate buffer for vertex data
	vertexData = new glm::vec3[pointsPerSectorLine * pointsPerSectorLine];

	// Generate buffer for indicesData
	indicesData.reserve((pointsPerSectorLine - 1) * (pointsPerSectorLine - 1) * 6);

	// Generate buffer for normalsData
	normalsData = new glm::vec3[pointsPerSectorLine * pointsPerSectorLine];
}


void WaterTerrain::initialize()
{
	ZoneScoped;

	// Init vertex data
	for (int i = 0; i < pointsPerSectorLine; i++)
	{
		for (int j = 0; j < pointsPerSectorLine; j++)
		{
			vertexData[(j + i * (pointsPerSectorLine))] = glm::vec3(i * (float)sectorSizeX / pointsPerSectorLine - sectorSizeX / 2, 0, j * (float)sectorSizeY / pointsPerSectorLine - sectorSizeY / 2);
		}
	}

	// Init indices data
	for (int i = 0; i < pointsPerSectorLine - 1; i++)
	{
		for (int j = 0; j < pointsPerSectorLine - 1; j++)
		{
			indicesData.push_back(j + i * pointsPerSectorLine);
			indicesData.push_back(j + (i + 1) * pointsPerSectorLine);
			indicesData.push_back((j + 1) + i * pointsPerSectorLine);

			indicesData.push_back((j + 1) + i * pointsPerSectorLine);
			indicesData.push_back(j + (i + 1) * pointsPerSectorLine);
			indicesData.push_back((j + 1) + (i + 1) * pointsPerSectorLine);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pointsPerSectorLine * pointsPerSectorLine, vertexData, GL_STATIC_DRAW);
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

	// Set uniforms
	shaderPtr->setVec4("wave[0]", wave1);
	shaderPtr->setVec4("wave[1]", wave2);
	shaderPtr->setVec4("wave[2]", wave3);
}


void WaterTerrain::update()
{
	ZoneScoped;
}

void WaterTerrain::sendAndRender()
{
	ZoneScoped;

	// bind global VAO object
	glBindVertexArray(mainVao.id);

	shaderPtr->setFloat("time", glfwGetTime());

	glDrawElements(
		GL_TRIANGLES,			// mode
		indicesData.size(),		// count
		GL_UNSIGNED_INT,		// type
		(void*)0				// element array buffer offset
	);
}

Shader* WaterTerrain::getShader()
{
	return shaderPtr;
}

WaterTerrain::~WaterTerrain()
{
	indicesData.clear();
	delete[] vertexData;
	delete[] normalsData;
}

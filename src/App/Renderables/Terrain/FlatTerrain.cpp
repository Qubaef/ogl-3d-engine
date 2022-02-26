#include "FlatTerrain.h"

using namespace glm;

FlatTerrain::FlatTerrain(int sectorSizeX, int sectorSizeY, int pointsPerSectorLine) :
	sectorSizeX(sectorSizeX),
	sectorSizeY(sectorSizeY),
	pointsPerSectorLine(pointsPerSectorLine)
{
	// Generate buffer for vertex data
	vertexData = new vec3[pointsPerSectorLine * pointsPerSectorLine];

	// Generate buffer for indicesData
	indicesData.reserve((pointsPerSectorLine - 1) * (pointsPerSectorLine - 1) * 6);

	// Generate buffer for normalsData
	normalsData = new vec3[pointsPerSectorLine * pointsPerSectorLine];
}


void FlatTerrain::initialize()
{
	ZoneScoped;

	// Init vertex data
	for (int i = 0; i < pointsPerSectorLine; i++)
	{
		for (int j = 0; j < pointsPerSectorLine; j++)
		{
			// vertexData[(j + i * (pointsPerSectorLine))] = vec3(i * (float)sectorSizeX / (pointsPerSectorLine - 1), 0, j * (float)sectorSizeY / (pointsPerSectorLine - 1));
			vertexData[(j + i * (pointsPerSectorLine))] = vec3(i * (float)sectorSizeX / pointsPerSectorLine - sectorSizeX / 2, 0.0, j * (float)sectorSizeY / pointsPerSectorLine - sectorSizeY / 2);
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

	// Init normals data
	for (int i = 0; i < pointsPerSectorLine; i++)
	{
		for (int j = 0; j < pointsPerSectorLine; j++)
		{
			vec3 current = vertexData[(j + i * (pointsPerSectorLine))];

			// Init normal vector
			vec3 normal = vec3(0, 0, 0);

			if (i > 0 && j < pointsPerSectorLine - 1)
			{
				normal += cross((vertexData[((j + 1) + (i - 1) * (pointsPerSectorLine))] - current), (current - vertexData[(j + (i - 1) * (pointsPerSectorLine))]));
			}

			if (j > 0 && i < pointsPerSectorLine - 1)
			{
				normal += cross((current - vertexData[((j - 1) + i * (pointsPerSectorLine))]), (vertexData[((j - 1) + (i + 1) * (pointsPerSectorLine))] - current));
			}

			if (i < pointsPerSectorLine - 1 && j < pointsPerSectorLine - 1)
			{
				normal += cross((current - vertexData[(j + (i + 1) * (pointsPerSectorLine))]), (vertexData[((j + 1) + i * (pointsPerSectorLine))] - current));
			}

			// Normalize vector's length
			if (length(normal) == 0)
			{
				normal.y = 1;
			}

			normalsData[(j + i * (pointsPerSectorLine))] = normalize(normal);
		}
	}

	// Attributes setup and global VAO creation

	// bind global VAO object
	glBindVertexArray(mainVao.id);

	// select indices VBO (to perform VBO indexing)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer.id);
	// bind indices VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(int), &indicesData[0], GL_DYNAMIC_DRAW);

	//// 1st vertex shader input attribute - vertex data
	// select vertex VBO
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * pointsPerSectorLine * pointsPerSectorLine, vertexData, GL_DYNAMIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * pointsPerSectorLine * pointsPerSectorLine, normalsData, GL_DYNAMIC_DRAW);
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

void FlatTerrain::update()
{
	ZoneScoped;

	////// Init vertex data
	//for (int i = 0; i < pointsPerSectorLine; i++)
	//{
	//	for (int j = 0; j < pointsPerSectorLine; j++)
	//	{
	//		vertexData[(j + i * (pointsPerSectorLine))] = vec3(i * (float)sectorSizeX / pointsPerSectorLine - sectorSizeX / 2, (float)std::rand() / RAND_MAX, j * (float)sectorSizeY / pointsPerSectorLine - sectorSizeY / 2);
	//	}
	//}

	////// Init normals data
	//for (int i = 0; i < pointsPerSectorLine; i++)
	//{
	//	for (int j = 0; j < pointsPerSectorLine; j++)
	//	{
	//		vec3 current = vertexData[(j + i * (pointsPerSectorLine))];

	//		// Init normal vector
	//		vec3 normal = vec3(0, 0, 0);

	//		if (i > 0 && j < pointsPerSectorLine - 1)
	//		{
	//			normal += cross((vertexData[((j + 1) + (i - 1) * (pointsPerSectorLine))] - current), (current - vertexData[(j + (i - 1) * (pointsPerSectorLine))]));
	//		}

	//		if (j > 0 && i < pointsPerSectorLine - 1)
	//		{
	//			normal += cross((current - vertexData[((j - 1) + i * (pointsPerSectorLine))]), (vertexData[((j - 1) + (i + 1) * (pointsPerSectorLine))] - current));
	//		}

	//		if (i < pointsPerSectorLine - 1 && j < pointsPerSectorLine - 1)
	//		{
	//			normal += cross((current - vertexData[(j + (i + 1) * (pointsPerSectorLine))]), (vertexData[((j + 1) + i * (pointsPerSectorLine))] - current));
	//		}

	//		// Normalize vector's length
	//		if (length(normal) == 0)
	//		{
	//			normal.y = 1;
	//		}

	//		normalsData[(j + i * (pointsPerSectorLine))] = normalize(normal);
	//	}
	//}
}


void FlatTerrain::sendAndRender(Shader* shaderPtr)
{
	// bind global VAO object
	glBindVertexArray(mainVao.id);

	// Set material
	shaderPtr->setVec3("material.ambient", vec3(0.298f, 0.282f, 0.27f));
	shaderPtr->setVec3("material.diffuse", vec3(0.458f, 0.411f, 0.341f));
	shaderPtr->setVec3("material.specular", vec3(1.0f, 1.0f, 1.0f));
	shaderPtr->setFloat("material.shininess", 32);

	// Draw
	glDrawElements(
		GL_TRIANGLES,			// mode
		indicesData.size(),		// count
		GL_UNSIGNED_INT,		// type
		(void*)0				// element array buffer offset
	);
}

FlatTerrain::~FlatTerrain()
{
	glDeleteBuffers(1, &vertexBuffer.id);
	glDeleteBuffers(1, &normalsBuffer.id);
	glDeleteBuffers(1, &indicesBuffer.id);
	indicesData.clear();
	delete[] vertexData;
	delete[] normalsData;
}

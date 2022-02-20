#include "InteriorNoShading.h"

#include <array>

#include "GuiEntityManager/EntityProperties/FloatPropertyContinuousModifier.h"
#include "GuiEntityManager/EntityProperties/IntPropertyContinuousDragModifier.h"
#include "GuiEntityManager/EntityProperties/Vec3PropertyContinuousModifier.h"
#include "GuiEntityManager/Messages/RegisterEntityMessage.h"
#include "GuiEntityManager/Messages/RegisterPropertyMessage.h"
#include "Renderer/OpenGL/Utils.h"
#include "Renderer/OpenGL/Validation.h"

using namespace glm;

InteriorNoShading::InteriorNoShading(Engine& engine) :
	IProcessable(engine),
	IMessanger(&engine.getMessageBus(), "InteriorNoShading"),
	pos(0, 0, 0)
{
	sendMessage(new RegisterEntityMessage(""), "EntityManager");

	depthShader = engine.getShaderByName("DepthShader");
	lightingShadowsShader = engine.getShaderByName("LightingShadowsShader");

	generateFloor();
	generateWall(vec2(-1, 0), vec3(1, 0, 0));
	generateWall(vec2(1, 0), vec3(-1, 0, 0));
	generateWall(vec2(0, -1), vec3(0, 0, 1));
	generateWall(vec2(0, 1), vec3(0, 0, -1));

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
		0,			// location in shader
		3,			// size
		GL_FLOAT,	// type
		GL_FALSE,	// normalized
		0,			// stride (space between following fragments)
		(void*)0	// array buffer offset
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
		1,			// location in shader
		3,			// size
		GL_FLOAT,	// type
		GL_FALSE,	// normalized
		0,			// stride (space between following fragments)
		(void*)0	// array buffer offset
	);
	// enable attribute '1'
	glEnableVertexAttribArray(1);

	// Generate box
	generateBox(vec3(0, 5, 0), 10);
	generateBox(vec3(-10, 1, -10), 2);
	generateBox(vec3(10, 3, 15), 6);

	// Render box
	// bind global VAO object
	glBindVertexArray(boxesVao.id);

	// select indices VBO (to perform VBO indexing)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer2.id);
	// bind indices VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData2.size() * sizeof(int), &indicesData2[0], GL_STATIC_DRAW);

	//// 1st vertex shader input attribute - vertex data
	// select vertex VBO
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2.id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, vertexData2.size() * sizeof(vec3), &vertexData2[0], GL_STATIC_DRAW);
	// redirect buffer to input of the shader
	glVertexAttribPointer(
		0,			// location in shader
		3,			// size
		GL_FLOAT,	// type
		GL_FALSE,	// normalized
		0,			// stride (space between following fragments)
		(void*)0	// array buffer offset
	);
	// enable attribute '0'
	glEnableVertexAttribArray(0);

	// 2nd vertex shader input attribute - normals
	// select normals VBO
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer2.id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, normalsData2.size() * sizeof(vec3), &normalsData2[0], GL_STATIC_DRAW);
	// redirect buffer to input of the shader
	glVertexAttribPointer(
		1,			// location in shader
		3,			// size
		GL_FLOAT,	// type
		GL_FALSE,	// normalized
		0,			// stride (space between following fragments)
		(void*)0	// array buffer offset
	);
	// enable attribute '1'
	glEnableVertexAttribArray(1);
}

void InteriorNoShading::preprocess()
{
}

void InteriorNoShading::process()
{
}

void InteriorNoShading::render()
{
	if((engine.getShaderGlobalData().data.displayMode & ShaderGlobalData::DisplayMode::DEPTH) > 0)
	{
		setupDepthRender();
	}
	else if((engine.getShaderGlobalData().data.displayMode & ShaderGlobalData::DisplayMode::DEFAULT) > 0)
	{
		setupDefaultRender();
	}

	renderScene();
}

void InteriorNoShading::generateBox(vec3 pos, int scale)
{
	int d = scale / 2;
	int indicesOffset = vertexData2.size();

	// Init top face in pos
	vertexData2.emplace_back(pos.x - d, pos.y + d, pos.z - d);
	vertexData2.emplace_back(pos.x + d, pos.y + d, pos.z - d);
	vertexData2.emplace_back(pos.x + d, pos.y + d, pos.z + d);
	vertexData2.emplace_back(pos.x - d, pos.y + d, pos.z + d);

	// Init normals for top face
	normalsData2.emplace_back(0, 1, 0);
	normalsData2.emplace_back(0, 1, 0);
	normalsData2.emplace_back(0, 1, 0);
	normalsData2.emplace_back(0, 1, 0);

	// Init indices for top face
	indicesData2.emplace_back(indicesOffset + 0);
	indicesData2.emplace_back(indicesOffset + 1);
	indicesData2.emplace_back(indicesOffset + 2);
	indicesData2.emplace_back(indicesOffset + 0);
	indicesData2.emplace_back(indicesOffset + 2);
	indicesData2.emplace_back(indicesOffset + 3);


	// Init bottom face in pos
	vertexData2.emplace_back(pos.x - d, pos.y - d, pos.z - d);
	vertexData2.emplace_back(pos.x + d, pos.y - d, pos.z - d);
	vertexData2.emplace_back(pos.x + d, pos.y - d, pos.z + d);
	vertexData2.emplace_back(pos.x - d, pos.y - d, pos.z + d);

	// Init normals for bottom face
	normalsData2.emplace_back(0, -1, 0);
	normalsData2.emplace_back(0, -1, 0);
	normalsData2.emplace_back(0, -1, 0);
	normalsData2.emplace_back(0, -1, 0);

	// Init indices for bottom face
	indicesData2.emplace_back(indicesOffset + 4);
	indicesData2.emplace_back(indicesOffset + 5);
	indicesData2.emplace_back(indicesOffset + 6);
	indicesData2.emplace_back(indicesOffset + 4);
	indicesData2.emplace_back(indicesOffset + 6);
	indicesData2.emplace_back(indicesOffset + 7);


	// Init front face in pos
	vertexData2.emplace_back(pos.x - d, pos.y - d, pos.z + d);
	vertexData2.emplace_back(pos.x + d, pos.y - d, pos.z + d);
	vertexData2.emplace_back(pos.x + d, pos.y + d, pos.z + d);
	vertexData2.emplace_back(pos.x - d, pos.y + d, pos.z + d);

	// Init normals for front face
	normalsData2.emplace_back(0, 0, 1);
	normalsData2.emplace_back(0, 0, 1);
	normalsData2.emplace_back(0, 0, 1);
	normalsData2.emplace_back(0, 0, 1);

	// Init indices for front face
	indicesData2.emplace_back(indicesOffset + 8);
	indicesData2.emplace_back(indicesOffset + 10);
	indicesData2.emplace_back(indicesOffset + 9);
	indicesData2.emplace_back(indicesOffset + 8);
	indicesData2.emplace_back(indicesOffset + 11);
	indicesData2.emplace_back(indicesOffset + 10);


	// Init back face in pos
	vertexData2.emplace_back(pos.x - d, pos.y - d, pos.z - d);
	vertexData2.emplace_back(pos.x + d, pos.y - d, pos.z - d);
	vertexData2.emplace_back(pos.x + d, pos.y + d, pos.z - d);
	vertexData2.emplace_back(pos.x - d, pos.y + d, pos.z - d);

	// Init normals for back face
	normalsData2.emplace_back(0, 0, -1);
	normalsData2.emplace_back(0, 0, -1);
	normalsData2.emplace_back(0, 0, -1);
	normalsData2.emplace_back(0, 0, -1);

	// Init indices for back face
	indicesData2.emplace_back(indicesOffset + 12);
	indicesData2.emplace_back(indicesOffset + 13);
	indicesData2.emplace_back(indicesOffset + 14);
	indicesData2.emplace_back(indicesOffset + 12);
	indicesData2.emplace_back(indicesOffset + 14);
	indicesData2.emplace_back(indicesOffset + 15);


	// Init left face in pos
	vertexData2.emplace_back(pos.x - d, pos.y - d, pos.z - d);
	vertexData2.emplace_back(pos.x - d, pos.y - d, pos.z + d);
	vertexData2.emplace_back(pos.x - d, pos.y + d, pos.z + d);
	vertexData2.emplace_back(pos.x - d, pos.y + d, pos.z - d);

	// Init normals for left face
	normalsData2.emplace_back(-1, 0, 0);
	normalsData2.emplace_back(-1, 0, 0);
	normalsData2.emplace_back(-1, 0, 0);
	normalsData2.emplace_back(-1, 0, 0);

	// Init indices for left face
	indicesData2.emplace_back(indicesOffset + 16);
	indicesData2.emplace_back(indicesOffset + 18);
	indicesData2.emplace_back(indicesOffset + 17);
	indicesData2.emplace_back(indicesOffset + 16);
	indicesData2.emplace_back(indicesOffset + 19);
	indicesData2.emplace_back(indicesOffset + 18);


	// Init right face in pos
	vertexData2.emplace_back(pos.x + d, pos.y - d, pos.z - d);
	vertexData2.emplace_back(pos.x + d, pos.y - d, pos.z + d);
	vertexData2.emplace_back(pos.x + d, pos.y + d, pos.z + d);
	vertexData2.emplace_back(pos.x + d, pos.y + d, pos.z - d);

	// Init normals for right face
	normalsData2.emplace_back(1, 0, 0);
	normalsData2.emplace_back(1, 0, 0);
	normalsData2.emplace_back(1, 0, 0);
	normalsData2.emplace_back(1, 0, 0);

	// Init indices for right face
	indicesData2.emplace_back(indicesOffset + 20);
	indicesData2.emplace_back(indicesOffset + 21);
	indicesData2.emplace_back(indicesOffset + 22);
	indicesData2.emplace_back(indicesOffset + 20);
	indicesData2.emplace_back(indicesOffset + 22);
	indicesData2.emplace_back(indicesOffset + 23);
}

void InteriorNoShading::generateFloor()
{
	// Generate the vertex data of the floor
	// Init indices data
	for (int i = 0; i < meshDensity; i++)
	{
		for (int j = 0; j < meshDensity; j++)
		{
			indicesData.push_back(vertexData.size() + j + i * (meshDensity + 1));
			indicesData.push_back(vertexData.size() + j + (i + 1) * (meshDensity + 1));
			indicesData.push_back(vertexData.size() + (j + 1) + i * (meshDensity + 1));

			indicesData.push_back(vertexData.size() + (j + 1) + i * (meshDensity + 1));
			indicesData.push_back(vertexData.size() + j + (i + 1) * (meshDensity + 1));
			indicesData.push_back(vertexData.size() + (j + 1) + (i + 1) * (meshDensity + 1));
		}
	}

	for (int i = 0; i <= meshDensity; i++)
	{
		for (int j = 0; j <= meshDensity; j++)
		{
			vertexData.emplace_back(
				pos.x - (scaleX / 2) + (i * scaleX) / meshDensity,
				pos.y,
				pos.z - (scaleZ / 2) + (j * scaleZ) / meshDensity
			);

			normalsData.emplace_back(0, 1, 0);
		}
	}
}

void InteriorNoShading::generateWall(vec2 wall, vec3 direction)
{
	// Generate the vertex data of the -x wall
	// Init indices data
	for (int i = 0; i < meshDensity; i++)
	{
		for (int j = 0; j < meshDensity; j++)
		{
			indicesData.push_back(vertexData.size() + j + i * (meshDensity + 1));
			indicesData.push_back(vertexData.size() + j + (i + 1) * (meshDensity + 1));
			indicesData.push_back(vertexData.size() + (j + 1) + i * (meshDensity + 1));

			indicesData.push_back(vertexData.size() + (j + 1) + i * (meshDensity + 1));
			indicesData.push_back(vertexData.size() + j + (i + 1) * (meshDensity + 1));
			indicesData.push_back(vertexData.size() + (j + 1) + (i + 1) * (meshDensity + 1));
		}
	}

	// Generate the vertices data of the wall of given axis
	// wallHeight is the height of the wall
	for (int i = 0; i <= meshDensity; i++)
	{
		for (int j = 0; j <= meshDensity; j++)
		{
			if (wall.x != 0 && wall.y == 0)
			{
				vertexData.emplace_back(
					pos.x + wall.x * (scaleX / 2) - wall.y * (i * scaleX) / meshDensity,
					pos.y + (wallHeight * i) / meshDensity,
					pos.z - wall.x * (scaleZ / 2) + wall.x * (j * scaleZ) / meshDensity
				);
			}
			else if (wall.x == 0 && wall.y != 0)
			{
				vertexData.emplace_back(
					pos.x + wall.y * (scaleX / 2) - wall.y * (j * scaleX) / meshDensity,
					pos.y + (wallHeight * i) / meshDensity,
					pos.z + wall.y * (scaleZ / 2) - wall.x * (i * scaleZ) / meshDensity
				);
			}

			normalsData.emplace_back(direction);
		}
	}
}

void InteriorNoShading::setupDepthRender()
{
	// Render scene from light's point of view
	depthShader->use();
}

void InteriorNoShading::setupDefaultRender()
{
	// Render scene from camera's point of view
	lightingShadowsShader->use();

	lightingShadowsShader->setVec3("material.ambient", vec3(0.0215, 0.1745, 0.0215));
	lightingShadowsShader->setVec3("material.diffuse", vec3(0.07568, 0.61424, 0.07568));
	lightingShadowsShader->setVec3("material.specular", vec3(0.633, 0.727811, 0.633));
	lightingShadowsShader->setFloat("material.shininess", 2);
}

void InteriorNoShading::renderScene()
{
	// Bind global VAO object
	glBindVertexArray(mainVao.id);

	// Draw
	glDrawElements(
		GL_TRIANGLES,			// mode
		indicesData.size(),		// count
		GL_UNSIGNED_INT,		// type
		0						// element array buffer offset
	);

	// Bind global VAO object
	glBindVertexArray(boxesVao.id);

	// Draw
	glDrawElements(
		GL_TRIANGLES,			// mode
		indicesData2.size(),	// count
		GL_UNSIGNED_INT,		// type
		0						// element array buffer offset
	);
}

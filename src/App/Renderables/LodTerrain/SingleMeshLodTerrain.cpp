#include "SingleMeshLodTerrain.h"

#include "Engine/Engine.h"

#include "App/Renderables/GuiEntityManager/Messages/RegisterPropertyMessage.h"
#include "App/Renderables/GuiEntityManager/Messages/RegisterEntityMessage.h"
#include "App/Renderables/GuiEntityManager/Messages/OnPropertyChangeMessage.h"
#include "App/Renderables/GuiEntityManager/EntityProperties/IntPropertyContinuousDragModifier.h"
#include "App/Renderables/GuiEntityManager/EntityProperties/Vec3PropertyContinuousModifier.h"
#include "App/Renderables/GuiEntityManager/EntityProperties/IntPropertyNotifyModifier.h"

using namespace glm;

SingleMeshLodTerrain::SingleMeshLodTerrain(Engine& engine, int size, int density, int minLodPatchSize)
	: IProcessable(engine), IMessanger(&engine.getMessageBus(), "SingleMeshLodTerrain"),
	size(size), density(density), minLodPatchSize(minLodPatchSize)
{
	shaderPtr = engine.getShaderByName("TerrainLod");

	sendMessage(new RegisterEntityMessage(""), "EntityManager");

	sendMessage(new RegisterPropertyMessage("SingleMeshLodTerrain",
		new Vec3PropertyContinuousModifier("terrainOffset", -1000, 1000, terrainOffset)),
		"EntityManager");

	sendMessage(new RegisterPropertyMessage("SingleMeshLodTerrain",
		new IntPropertyNotifyModifier("SingleMeshLodTerrain", "notify variable", -1000, 1000, 0)),
		"EntityManager");

	// Initialize terrainQuadTree
	terrainQuadTree = new QuadTerrainNode(-size * 0.5f, -size * 0.5f, size);

	// Initialize view matrices
	shaderPtr->use();

	// Get uniforms handles
	uniformId_matMvp = glGetUniformLocation(shaderPtr->getId(), "matMvp");
	uniformId_matMv = glGetUniformLocation(shaderPtr->getId(), "matMv");
	uniformId_matM = glGetUniformLocation(shaderPtr->getId(), "matM");
	uniformId_matP = glGetUniformLocation(shaderPtr->getId(), "matP");

	uniformId_nodePos = glGetUniformLocation(shaderPtr->getId(), "nodePos");
	uniformId_nodeSize = glGetUniformLocation(shaderPtr->getId(), "nodeSize");

	uniformId_viewport = glGetUniformLocation(shaderPtr->getId(), "viewport");
	uniformId_terrainHeight = glGetUniformLocation(shaderPtr->getId(), "terrainHeight");

	uniformId_terrainDensity = glGetUniformLocation(shaderPtr->getId(), "terrainDensity");
	uniformId_terrainSize = glGetUniformLocation(shaderPtr->getId(), "terrainSize");
	uniformId_terrainOffset = glGetUniformLocation(shaderPtr->getId(), "terrainOffset");

	uniformId_viewPos = glGetUniformLocation(shaderPtr->getId(), "viewPos");

	// Init vertex data
	vertexData[0] = glm::vec3(0, 0, 0);
	vertexData[1] = glm::vec3(0, 0, 1);
	vertexData[2] = glm::vec3(1, 0, 0);
	vertexData[3] = glm::vec3(1, 0, 1);

	// Init normals data
	normalsData[0] = glm::vec3(0, 1, 0);
	normalsData[1] = glm::vec3(0, 1, 0);
	normalsData[2] = glm::vec3(0, 1, 0);
	normalsData[3] = glm::vec3(0, 1, 0);

	// Init indices data
	indicesData[0] = 0;
	indicesData[1] = 1;
	indicesData[2] = 2;
	indicesData[3] = 3;

	// Initialize heightmap
	if (true)
	{
		heightmap = new Heightmap(size, density);
		heightmap->updateGpu(engine);
		// heightmap->update();
		// heightmap->saveToFile();
		// heightmap->update(noise);
		// heightmap = new Heightmap(size, 2 * size);
		// heightmap->update(noise);
		// heightmap->saveToFile();
	}
	else
	{
		heightmap = new Heightmap("Heightmap.tga");
	}

	// bind global VAO object
	glBindVertexArray(vao_main.id);

	// select indices VBO (to perform VBO indexing)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices.id);
	// bind indices VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData), &indicesData[0], GL_DYNAMIC_DRAW);

	//// 1st vertex shader input attribute - vertex data
		// select vertex VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex.id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 4, vertexData, GL_DYNAMIC_DRAW);
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
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals.id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 4, normalsData, GL_DYNAMIC_DRAW);
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

void SingleMeshLodTerrain::preprocess()
{
}

void SingleMeshLodTerrain::process()
{
	// Acquire all messages from the message bus
	const auto messages = getMessages();

	for (Message* message : messages)
	{
		if (OnPropertyChangeMessage* propertyChangeMessage = dynamic_cast<OnPropertyChangeMessage*>(message))
		{
			if(strcmp(propertyChangeMessage->getName(), "notify variable") == 0)
			{
				LOG.INFO("'notify variable' is now: %d\n", propertyChangeMessage->getValue());
			}
			continue;
		}
	}

	// Update Nodes
	glm::vec3 pos = engine.getCamera()->getPosition();
	pos.y = 0;

	terrainQuadTree->update(pos, minLodPatchSize);
}

void SingleMeshLodTerrain::render()
{
	ZoneScoped;

	Camera* cameraPtr = engine.getCamera();

	// Get uniforms data
	glm::mat4* Mvp = cameraPtr->getMVPMatrix();
	glm::mat4* M = cameraPtr->getModelMatrix();
	glm::mat4* P = cameraPtr->getProjectionMatrix();
	glm::mat4 Mv = *(cameraPtr->getViewMatrix()) * (*M);

	int viewportW = engine.props.consts.windowWidth;
	int viewportH = engine.props.consts.windowHeight;

	vec3 viewPos = cameraPtr->getPosition();

	// Update all uniforms
	shaderPtr->use();

	glUniformMatrix4fv(uniformId_matMvp, 1, GL_FALSE, &(*Mvp)[0][0]);
	glUniformMatrix4fv(uniformId_matMv, 1, GL_FALSE, &(Mv)[0][0]);
	glUniformMatrix4fv(uniformId_matM, 1, GL_FALSE, &(*M)[0][0]);
	glUniformMatrix4fv(uniformId_matP, 1, GL_FALSE, &(*P)[0][0]);

	glUniform2f(uniformId_viewport, viewportW, viewportH);
	glUniform1i(uniformId_terrainHeight, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightmap->textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glUniform1f(uniformId_terrainDensity, density);
	glUniform1f(uniformId_terrainSize, size);
	glUniform3f(uniformId_terrainOffset, terrainOffset.x, terrainOffset.y, terrainOffset.z);

	glUniform3f(uniformId_viewPos, viewPos.x, viewPos.y, viewPos.z);

	// Set materials
	shaderPtr->setVec3("materialSteep.ambient", vec3(0.298f, 0.282f, 0.27f));
	shaderPtr->setVec3("materialSteep.diffuse", vec3(0.458f, 0.411f, 0.341f));
	shaderPtr->setVec3("materialSteep.specular", vec3(0.0f, 0.0f, 0.0f));
	shaderPtr->setFloat("materialSteep.shininess", 1024);

	shaderPtr->setVec3("materialFlat.ambient", vec3(0.339f, 0.559f, 0.339f) * vec3(0.7));
	shaderPtr->setVec3("materialFlat.diffuse", vec3(0.565f, 0.933f, 0.565f) * vec3(0.7));
	shaderPtr->setVec3("materialFlat.specular", vec3(0.0f, 0.0f, 0.0f));
	shaderPtr->setFloat("materialFlat.shininess", 1024);

	// Attributes setup and global VAO creation
	// bind global VAO object
	glBindVertexArray(vao_main.id);

	// Specify patch size
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	// Render whole Terrain Tree
	terrainQuadTree->render(shaderPtr, uniformId_nodePos, uniformId_nodeSize, engine);
}
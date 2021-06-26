#include "LodTerrain.h"

#include "Engine/Engine.h"

LodTerrain::LodTerrain(Engine* enginePtr)
	: Processable(enginePtr)
{
	ZoneScoped;

	shaderPtr = enginePtr->getShaderByName("TerrainLod");

	// Initialize terrainQuadTree
	terrainQuadTree = new QuadTerrainNode(-size / 2, -size / 2, size);

	// Initialize view matrices
	shaderPtr->use();

	// Get uniforms handles
	matMvpId = glGetUniformLocation(shaderPtr->get_ID(), "matMvp");
	matMvId = glGetUniformLocation(shaderPtr->get_ID(), "matMv");
	matMId = glGetUniformLocation(shaderPtr->get_ID(), "matM");
	matPId = glGetUniformLocation(shaderPtr->get_ID(), "matP");

	nodePosId = glGetUniformLocation(shaderPtr->get_ID(), "nodePos");
	nodeSizeId = glGetUniformLocation(shaderPtr->get_ID(), "nodeSize");

	viewportId = glGetUniformLocation(shaderPtr->get_ID(), "viewport");
	terrainHeightId = glGetUniformLocation(shaderPtr->get_ID(), "terrainHeight");
	// terrainHeigh2tId = glGetUniformLocation(shaderPtr->get_ID(), "terrainHeight2");
	terrainHeightOffsetId = glGetUniformLocation(shaderPtr->get_ID(), "terrainHeightOffset");

	tscaleNegxId = glGetUniformLocation(shaderPtr->get_ID(), "tscaleNegx");
	tscaleNegzId = glGetUniformLocation(shaderPtr->get_ID(), "tscaleNegz");
	tscalePosxId = glGetUniformLocation(shaderPtr->get_ID(), "tscalePosx");
	tscalePoszId = glGetUniformLocation(shaderPtr->get_ID(), "tscalePosz");

	viewPosId = glGetUniformLocation(shaderPtr->get_ID(), "viewPos");

	// Prepare vertex data
	vertexData[0] = glm::vec3(0, 0, 0);
	vertexData[1] = glm::vec3(0, 0, 1);
	vertexData[2] = glm::vec3(1, 0, 0);
	vertexData[3] = glm::vec3(1, 0, 1);

	// Prepare normals data
	normalsData[0] = glm::vec3(0, 1, 0);
	normalsData[1] = glm::vec3(0, 1, 0);
	normalsData[2] = glm::vec3(0, 1, 0);
	normalsData[3] = glm::vec3(0, 1, 0);

	// Prepare indices data
	indicesData[0] = 0;
	indicesData[1] = 2;
	indicesData[2] = 1;
	indicesData[3] = 1;
	indicesData[4] = 2;
	indicesData[5] = 3;

	enginePtr->checkOglErrors("Lodterrain-preheightmap");

	// Initialize heightmap
	if (true)
	{
		heightmap = new Heightmap(size, 2 * size);
		heightmap->updateGpu(enginePtr);
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

	enginePtr->checkOglErrors("Lodterrain-prebuffers");

	// bind global VAO object
	glBindVertexArray(mainVao.id);

	// select indices VBO (to perform VBO indexing)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer.id);
	// bind indices VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData), &indicesData[0], GL_DYNAMIC_DRAW);

	//// 1st vertex shader input attribute - vertex data
		// select vertex VBO
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.id);
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
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer.id);
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

void LodTerrain::preprocess()
{

}

void LodTerrain::process()
{
	ZoneScoped;

	// Update Nodes
	Camera* cameraPtr = enginePtr->getCamera();
	glm::vec3 pos = cameraPtr->getPosition();
	float minSize = 20;

	terrainQuadTree->update(pos, minSize);
}

void LodTerrain::render()
{
	ZoneScoped;

	Camera* cameraPtr = enginePtr->getCamera();

	// Get uniforms data
	glm::mat4* Mvp = cameraPtr->getMVPMatrix();
	glm::mat4* M = cameraPtr->getModelMatrix();
	glm::mat4* P = cameraPtr->getProjectionMatrix();
	glm::mat4 Mv = (*cameraPtr->getViewMatrix()) * (*M);

	int viewportW = enginePtr->getConstProperties().windowWidth;
	int viewportH = enginePtr->getConstProperties().windowHeight;

	vec3 viewPos = cameraPtr->getPosition();

	// Update all uniforms
	shaderPtr->use();

	enginePtr->checkOglErrors("Lodterrain-prematrix");

	glUniformMatrix4fv(matMvpId, 1, GL_FALSE, &(*Mvp)[0][0]);
	glUniformMatrix4fv(matMvId, 1, GL_FALSE, &(Mv)[0][0]);
	glUniformMatrix4fv(matMId, 1, GL_FALSE, &(*M)[0][0]);
	glUniformMatrix4fv(matPId, 1, GL_FALSE, &(*P)[0][0]);

	enginePtr->checkOglErrors("Lodterrain-previewport");

	glUniform2f(viewportId, viewportW, viewportH);
	glUniform1i(terrainHeightId, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightmap->textureId);
	glUniform1f(terrainHeightOffsetId, 0.0);

	enginePtr->checkOglErrors("Lodterrain-pretess");

	glUniform1f(tscaleNegxId, 1.0);
	glUniform1f(tscaleNegzId, 1.0);
	glUniform1f(tscalePosxId, 1.0);
	glUniform1f(tscalePoszId, 1.0);

	glUniform3f(viewPosId, viewPos.x, viewPos.y, viewPos.z);

	enginePtr->checkOglErrors("Lodterrain-prematerial");

	// Set material
	shaderPtr->set_vec3("material.ambient", vec3(0.298f, 0.282f, 0.27f));
	shaderPtr->set_vec3("material.diffuse", vec3(0.458f, 0.411f, 0.341f));
	shaderPtr->set_vec3("material.specular", vec3(1.0f, 1.0f, 1.0f));
	shaderPtr->set_float("material.shininess", 32);

	// Attributes setup and global VAO creation
	// bind global VAO object
	glBindVertexArray(mainVao.id);



	enginePtr->checkOglErrors("Lodterrain-prerender");

	// Render whole Terrain Tree
	terrainQuadTree->render(shaderPtr, nodePosId, nodeSizeId, enginePtr);
}
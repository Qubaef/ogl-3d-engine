#include "Interior.h"

#include <array>
#include <stb_image_write.h>

#include "Engine/Common/ImageLibWrapper.h"
#include "GuiEntityManager/EntityProperties/FloatPropertyContinuousModifier.h"
#include "GuiEntityManager/EntityProperties/IntPropertyContinuousModifier.h"
#include "GuiEntityManager/EntityProperties/Vec3PropertyContinuousModifier.h"
#include "GuiEntityManager/Messages/RegisterEntityMessage.h"
#include "GuiEntityManager/Messages/RegisterPropertyMessage.h"

void Interior::generateBox(vec3 pos, int scale)
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

void Interior::generateFloor()
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

void Interior::generateWall(vec2 wall, vec3 direction)
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

mat4 Interior::calculateLightSpaceMatrix()
{
	vec3 lightDir = enginePtr->getShaderManager()->getDirectionalLight().getDirection();
	Camera* cameraPtr = enginePtr->getCamera();

	mat4 lightProjection = ortho(
		-orthoSize, orthoSize, -orthoSize, orthoSize,
		nearPane,
		fearPane
	);

	vec3 targetPoint = cameraPtr->getPosition();
	mat4 lightView = lookAt(
		targetPoint + normalize(lightDir) * SHADOW_LIGHT_MULTIPLIER,
		// vec3(0, 0, 0),
		targetPoint,
		vec3(0.0f, 1.0f, 0.0f)
	);

	return lightProjection * lightView;
}

void Interior::setupDepthShaderAndMatrices()
{
	// Render scene from light's point of view
	shadowMappingShaderPtr->use();
	shadowMappingShaderPtr->set_mat4("lightSpaceMatrix", calculateLightSpaceMatrix());
}

void Interior::setupShaderAndMatrices()
{
	Camera* cameraPtr = enginePtr->getCamera();

	// Get pointers to matrices
	glm::mat4* mvp = cameraPtr->getMVPMatrix();
	glm::mat4* V = cameraPtr->getViewMatrix();
	glm::mat4* M = cameraPtr->getModelMatrix();

	shaderPtr->use();

	shaderPtr->set_vec3("material.ambient", vec3(0.0215, 0.1745, 0.0215));
	shaderPtr->set_vec3("material.diffuse", vec3(0.07568, 0.61424, 0.07568));
	shaderPtr->set_vec3("material.specular", vec3(0.633, 0.727811, 0.633));
	shaderPtr->set_float("material.shininess", 2);

	// Send transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(MvpMatrixID, 1, GL_FALSE, &(*mvp)[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &(*V)[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(*M)[0][0]);

	// Send M_inverted for optimization purposes (it is better to calculate it on cpu)
	shaderPtr->set_mat3("M_inverted", mat3(transpose(inverse(*M))));

	// Send view position for specular component
	shaderPtr->set_vec3("view_pos", cameraPtr->getPosition());

	// Render scene from light's point of view
	shaderPtr->use();
	shaderPtr->set_mat4("lightSpaceMatrix", calculateLightSpaceMatrix());
}

void Interior::renderScene()
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

void Interior::renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

Interior::Interior(Engine* enginePtr)
	: IProcessable(enginePtr),
	IMessanger(&enginePtr->getMessageBus(), "Interior"),
	pos(0, 0, 0)

{
	sendMessage(new RegisterEntityMessage(""), "EntityManager");

	sendMessage(new RegisterPropertyMessage("Interior",
		new FloatPropertyContinuousModifier("SHADOW_LIGHT_MULTIPLIER", 0, 500, SHADOW_LIGHT_MULTIPLIER, SHADOW_LIGHT_MULTIPLIER)),
		"EntityManager");

	sendMessage(new RegisterPropertyMessage("Interior",
		new FloatPropertyContinuousModifier("orthoSize", 0, 1000, orthoSize, orthoSize)),
		"EntityManager");

	sendMessage(new RegisterPropertyMessage("Interior",
		new FloatPropertyContinuousModifier("nearPane", 0, 500, nearPane, nearPane)),
		"EntityManager");

	sendMessage(new RegisterPropertyMessage("Interior",
		new FloatPropertyContinuousModifier("fearPane", 0, 1000, fearPane, fearPane)),
		"EntityManager");

	//sendMessage(new RegisterPropertyMessage("Interior",
	//	new IntPropertyContinuousModifier("SHADOW_LIGHT_MULTIPLIER", 0, 500, SHADOW_LIGHT_MULTIPLIER, SHADOW_LIGHT_MULTIPLIER)),
	//	"EntityManager");

	// shaderPtr = enginePtr->getShaderByName("MainShader");
	shaderPtr = enginePtr->getShaderByName("LightingShadowsShader");
	framebufferShaderPtr = enginePtr->getShaderByName("DepthShader");
	shadowMappingShaderPtr = enginePtr->getShaderByName("ShadowMappingShader");

	// Initialize view matrices
	shaderPtr->use();

	MvpMatrixID = glGetUniformLocation(shaderPtr->get_ID(), "MVP");
	ViewMatrixID = glGetUniformLocation(shaderPtr->get_ID(), "V");
	ModelMatrixID = glGetUniformLocation(shaderPtr->get_ID(), "M");

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
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(glm::vec3), &vertexData[0], GL_STATIC_DRAW);
	// redirect buffer to input of the shader
	glVertexAttribPointer(
		0, // location in shader
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized
		0, // stride (space between following fragments)
		(void*)0 // array buffer offset
	);
	// enable attribute '0'
	glEnableVertexAttribArray(0);

	// 2nd vertex shader input attribute - normals
	// select normals VBO
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer.id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, normalsData.size() * sizeof(glm::vec3), &normalsData[0], GL_STATIC_DRAW);
	// redirect buffer to input of the shader
	glVertexAttribPointer(
		1, // location in shader
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized
		0, // stride (space between following fragments)
		(void*)0 // array buffer offset
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
	glBufferData(GL_ARRAY_BUFFER, vertexData2.size() * sizeof(glm::vec3), &vertexData2[0], GL_STATIC_DRAW);
	// redirect buffer to input of the shader
	glVertexAttribPointer(
		0, // location in shader
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized
		0, // stride (space between following fragments)
		(void*)0 // array buffer offset
	);
	// enable attribute '0'
	glEnableVertexAttribArray(0);

	// 2nd vertex shader input attribute - normals
	// select normals VBO
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer2.id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, normalsData2.size() * sizeof(glm::vec3), &normalsData2[0], GL_STATIC_DRAW);
	// redirect buffer to input of the shader
	glVertexAttribPointer(
		1, // location in shader
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized
		0, // stride (space between following fragments)
		(void*)0 // array buffer offset
	);
	// enable attribute '1'
	glEnableVertexAttribArray(1);

	enginePtr->checkOglErrors("PreRender");


	// Shadows

	// Init framebuffer
	glGenFramebuffers(1, &depthMapFBO);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Generate texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
	//	SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	constexpr float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

	// Verify if Framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG.ERROR("Framebuffer is not complete!\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	enginePtr->checkOglErrors("PreRender");
}

void Interior::preprocess()
{
	// 1. first render to depth map
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	enginePtr->checkOglErrors("PreRender");
}

void Interior::process()
{
}

void Interior::render()
{
	enginePtr->checkOglErrors("PreRender");

	//// 1. first render to depth map
	setupDepthShaderAndMatrices();
	renderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Draw depth map's framebuffer to viewport
	if (false)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		framebufferShaderPtr->use();
		framebufferShaderPtr->set_float("near_plane", enginePtr->getConstProperties().cameraNearClipping);
		framebufferShaderPtr->set_float("far_plane", enginePtr->getConstProperties().cameraFarClipping);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderQuad();

		enginePtr->checkOglErrors("AfterFramebuffer");
	}
	
	// Dump framebuffer to file
	if (false)
	{
		float* data = new float[SHADOW_WIDTH * SHADOW_HEIGHT];
		// Read texture to data
		glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data);
		ImageLibWrapper::image_write_tga("depthmap.tga", SHADOW_WIDTH, SHADOW_HEIGHT, 4, data);
		delete[] data;
	}
	
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	if (true)
	{
		// 2. then render scene as normal with shadow mapping (using depth map)
		glViewport(0, 0,
			enginePtr->getConstProperties().windowWidth,
			enginePtr->getConstProperties().windowHeight
		);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		setupShaderAndMatrices();
		renderScene();

		enginePtr->checkOglErrors("AfterFramebuffer");
	}
}

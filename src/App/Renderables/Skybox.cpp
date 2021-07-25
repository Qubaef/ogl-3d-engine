#include "Skybox.h"

#include "Engine/Engine.h"
#include "Engine/Common/ImageLibWrapper.h"

float Skybox::skyboxVertices[108] = {
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};

Skybox::Skybox(Engine* enginePtr)
	: Processable(enginePtr)
{
	ZoneScoped;

	// Get skybox shader
	shaderPtr = enginePtr->getShaderByName("SkyboxShader");

	// Initialize view matrices
	shaderPtr->use();

	ProjectionMatrixID = glGetUniformLocation(shaderPtr->get_ID(), "P");
	ViewMatrixID = glGetUniformLocation(shaderPtr->get_ID(), "V");

	// Initialize Texture
	skyboxFilenames =
	{
		"res/skybox/miramar_right.tga",
		"res/skybox/miramar_left.tga",
		"res/skybox/miramar_top.tga",
		"res/skybox/miramar_bottom.tga",
		"res/skybox/miramar_front.tga",
		"res/skybox/miramar_back.tga"
		//"res/skybox/right.jpg",
		//"res/skybox/left.jpg",
		//"res/skybox/top.jpg",
		//"res/skybox/bottom.jpg",
		//"res/skybox/front.jpg",
		//"res/skybox/back.jpg"
	};

	// Bind global VAO object
	glBindVertexArray(mainVao.id);

	// Load texture
	glGenTextures(1, &skyboxTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);

	for (unsigned i = 0; i < skyboxFilenames.size(); i++)
	{
		unsigned char* data = ImageLibWrapper::image_load(skyboxFilenames[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			ImageLibWrapper::image_free(data);
		}
		else
		{
			LOG.ERROR("Skybox texture failed to load at path: %s\n", skyboxFilenames[i].c_str());
			ImageLibWrapper::image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Initialize skybox vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.id);

	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	// redirect buffer to input of the shader
	glVertexAttribPointer(
		0,						// location in shader
		3,						// size
		GL_FLOAT,				// type
		GL_FALSE,				// normalized
		3 * sizeof(float),		// stride (space between following fragments)
		(void*)0				// array buffer offset
	);
	// enable attribute '0'
	glEnableVertexAttribArray(0);

	shaderPtr->set_int("skybox", 0);
}


void Skybox::render()
{
	ZoneScoped;

	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);

	Camera* cameraPtr = enginePtr->getCamera();

	// Get pointers to matrices
	glm::mat4* P = cameraPtr->getProjectionMatrix();
	glm::mat4 V = glm::mat4(glm::mat3(*cameraPtr->getViewMatrix())); // remove translation from the view matrix

	shaderPtr->use();
	glBindVertexArray(mainVao.id);

	// Send transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &(*P)[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &(V)[0][0]);

	// glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthFunc(GL_LESS);
	glCullFace(GL_FRONT);
}

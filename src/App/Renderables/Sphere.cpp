#include "Sphere.h"

using namespace glm;

Sphere::Sphere(Engine& engine, vec3 pos)
	: IProcessable(engine), pos(pos)
{
	shaderPtr = engine.getShaderByName("LightingShader");

	// Initialize view matrices
	shaderPtr->use();

	MvpMatrixID = glGetUniformLocation(shaderPtr->getId(), "MVP");
	ViewMatrixID = glGetUniformLocation(shaderPtr->getId(), "V");
	ModelMatrixID = glGetUniformLocation(shaderPtr->getId(), "M");

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

void Sphere::render()
{
	ZoneScoped;

	Camera* cameraPtr = engine.getCamera();

	// Get pointers to matrices
	mat4* mvp = cameraPtr->getMVPMatrix();
	mat4* V = cameraPtr->getViewMatrix();
	mat4* M = cameraPtr->getModelMatrix();

	shaderPtr->use();

	this->shaderPtr->setVec3("material.ambient", vec3(0.0215, 0.1745, 0.0215));
	this->shaderPtr->setVec3("material.diffuse", vec3(0.07568, 0.61424, 0.07568));
	this->shaderPtr->setVec3("material.specular", vec3(0.633, 0.727811, 0.633));
	this->shaderPtr->setFloat("material.shininess", 76.8f);

	// Send transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(MvpMatrixID, 1, GL_FALSE, &(*mvp)[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &(*V)[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(*M)[0][0]);

	// Send M_inverted for optimization purposes (it is better to calculate it on cpu)
	shaderPtr->setMat3("M_inverted", mat3(transpose(inverse(*M))));

	// Send view position for specular component
	shaderPtr->setVec3("view_pos", cameraPtr->getPosition());

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

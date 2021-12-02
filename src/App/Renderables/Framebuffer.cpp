#include "Framebuffer.h"

// Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates
float Framebuffer::framebufferVertices[24] = {
// positions   // texCoords
-1.0f,  1.0f,  0.0f, 1.0f,
-1.0f, -1.0f,  0.0f, 0.0f,
1.0f, -1.0f,  1.0f, 0.0f,

-1.0f,  1.0f,  0.0f, 1.0f,
1.0f, -1.0f,  1.0f, 0.0f,
1.0f,  1.0f,  1.0f, 1.0f
};


Framebuffer::Framebuffer(Engine* enginePtr)
	: Processable(enginePtr)
{
	defineRender();

	// Initialize buffers
	glBindVertexArray(mainVao.id);
	glBindBuffer(GL_ARRAY_BUFFER, mainVbo.id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(framebufferVertices), &framebufferVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Initialize Framebuffer
	glGenFramebuffers(1, &Fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, Fbo);

	// Get shader
	shaderPtr = enginePtr->getShaderByName("FramebufferShader");
	shaderPtr->use();
	shaderPtr->set_int("screenTexture", 0);
	shaderPtr->set_int("depthTexture", 1);
	shaderPtr->set_vec2("viewport", enginePtr->getConstProperties().windowWidth, enginePtr->getConstProperties().windowHeight);
	shaderPtr->set_float("farPlane", enginePtr->getConstProperties().cameraFarClipping);

	// Initialize Framebuffer texture
	ConstProperties props = enginePtr->getConstProperties();

	glGenTextures(1, &texColorFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorFrameBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, props.windowWidth, props.windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorFrameBuffer, 0);

	glGenTextures(1, &texDepthFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, texDepthFrameBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, props.windowWidth, props.windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texDepthFrameBuffer, 0);

	// Verify if Framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG.ERROR("Framebuffer is not complete!\n");
	}

	// Bind to default Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::preprocess()
{
	ImGui::Begin("DepthOffField");

	// ImGui::SetWindowPos(ImVec2(300, 0));

	ImGui::SliderFloat("focusPoint", &focusPoint, 0, 50);
	ImGui::SliderFloat("focusScale", &focusScale, 0, 2);

	ImGui::End();

	// Bind buffer to capture all the following operations
	glBindFramebuffer(GL_FRAMEBUFFER, Fbo);
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glEnable(GL_CULL_FACE);
}

void Framebuffer::process()
{
}

void Framebuffer::render()
{
	// Bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind shader
	shaderPtr->use();
	shaderPtr->set_float("focusPoint", focusPoint);
	shaderPtr->set_float("focusScale", focusScale);

	// Bind global VAO object
	glBindVertexArray(mainVao.id);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texColorFrameBuffer);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texDepthFrameBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

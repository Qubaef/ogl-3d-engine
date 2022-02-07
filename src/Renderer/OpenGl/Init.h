#pragma once

#include "Include.h"

/**
 * @brief			Initialize GLFW with it's default parameters and window
 *
 * @param width		target window width
 * @param height	target window height
 * @param name		target window name
 * @param debug		enable debug mode
 *
 * @return			Pointer to GLFWwindow, or null if error
*/
inline GLFWwindow* initGlfw(int width, int height, const char* name, bool debug)
{
	// Initialize Glfw
	if (!glfwInit())
		return nullptr;

	// Initialize default Glfw parameters
	glfwWindowHint(GLFW_SAMPLES, 4);									// 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);						// We want OpenGL 4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);					// To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// We don't want the old OpenGL

	// Initialize debug parameters
	if (debug)
	{
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);					// Enable debug context
	}

	// Initialize and return Glfw window
	GLFWwindow* windowPtr = glfwCreateWindow(width, height, name, nullptr, nullptr);
	glfwMakeContextCurrent(windowPtr);

	return windowPtr;
}

/**
 * @brief				Initialize Glew library
 *
 * @return				true if succeeded, false otherwise
*/
inline bool initGlew()
{
	// Initialize Glew
	glewExperimental = true;												// Needed for core profile

	return (glewInit() == GLEW_OK);
}
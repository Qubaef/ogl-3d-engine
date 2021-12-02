#pragma once

struct ConstProperties
{
	// Engine execution mode
	enum EXECUTION_MODE
	{
		REGULAR,
		DEBUG,
		TESTS,
	};

	EXECUTION_MODE executionMode = DEBUG;
	
	// Window properties
	const char* windowName = "Engine";
	const int windowWidth = 1280;
	const int windowHeight = 720;

	// TODO: Move camera properties to camera class
	// Camera properties
	const unsigned cameraFov = 45;
	const float cameraNearClipping = 5.f;
	const float cameraFarClipping = 100000.f;

	// Lights properties
	const int maxPointLightsNumber = 16;
};
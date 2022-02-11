#pragma once

struct PropertiesConst
{
	// Engine execution mode
	enum class EXECUTION_MODE
	{
		RELEASE,
		DEBUG,
		TESTS,
	};

	EXECUTION_MODE executionMode = EXECUTION_MODE::DEBUG;

	// Window properties
	const char* windowName = "Engine";
	const int windowWidth = 2560;
	const int windowHeight = 1440;

	// Camera properties
	const unsigned cameraFov = 45;
	const float cameraNearClipping = 1.f;
	const float cameraFarClipping = 20000.f;

	// Lights properties
	const int maxPointLightsNumber = 16;

	// Global data properties
	const int GLOBAL_DATA_BIND_ID = 0;
};
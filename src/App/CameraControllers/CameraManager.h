#pragma once
#include <vector>

#include "../../Engine/CpuPipeline/IProcessable.h"
#include "CameraController.h"
#include "InputManager.h"

class CameraManager : public IProcessable
{
	const int CAMERA_TOGGLE_KEY = GLFW_KEY_T;

	InputManager* inputManagerPtr;
	std::vector<CameraController*> cameraControllersList;
	int currentCameraIndex = -1;

	void enableCamera(CameraController* cameraControllerPtr);

public:
	CameraManager(Engine* enginePtr, InputManager* inputManagerPtr);

	void addCamera(CameraController* cameraControllerPtr);

	void preprocess() override;
	void process() override;
	void render() override;

	~CameraManager() override = default;
};

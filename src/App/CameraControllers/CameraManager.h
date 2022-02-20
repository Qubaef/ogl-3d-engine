#pragma once

#include <vector>

#include "Engine/Include/Common.h"

#include "Engine/CpuPipeline/IProcessable.h"
#include "Engine/Components/MessageBus/IMessanger.h"

class CameraController;
class InputManager;

class CameraManager : public IProcessable, public IMessanger
{
	const int CAMERA_TOGGLE_KEY = GLFW_KEY_T;

	InputManager* inputManagerPtr;
	std::vector<CameraController*> cameraControllersList;
	int currentCameraIndex = -1;

	glm::vec3 cameraPosition;
	glm::vec3 cameraDirection;

	void enableCamera(CameraController* cameraControllerPtr);

public:
	CameraManager(Engine& engine, InputManager* inputManagerPtr);

	void addCamera(CameraController* cameraControllerPtr);

	void preprocess() override;
	void process() override;
	void render() override;

	~CameraManager() override = default;
};

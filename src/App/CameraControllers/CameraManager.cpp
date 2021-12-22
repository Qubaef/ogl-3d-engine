#include "CameraManager.h"

#include "Engine/Engine.h"

CameraManager::CameraManager(Engine* enginePtr, InputManager* inputManagerPtr) : IProcessable(enginePtr), inputManagerPtr(inputManagerPtr)
{
	inputManagerPtr->register_key_event(CAMERA_TOGGLE_KEY);
}

void CameraManager::enableCamera(CameraController* cameraControllerPtr)
{
	cameraControllerPtr->enable();
	enginePtr->registerCamera(cameraControllerPtr);
}

void CameraManager::addCamera(CameraController* cameraControllerPtr)
{
	cameraControllersList.push_back(cameraControllerPtr);

	if (currentCameraIndex == -1)
	{
		currentCameraIndex = 0;
		enableCamera(cameraControllerPtr);
	}
}

void CameraManager::preprocess()
{
	// If no camera present, skip processing
	if (currentCameraIndex == -1)
	{
		return;
	}

	// Toggle camera if user CAMERA_TOGGLE_KEY was pressed
	if (inputManagerPtr->getKeyState(CAMERA_TOGGLE_KEY) == JUST_PRESSED)
	{
		const int newCameraIndex = (currentCameraIndex + 1) % cameraControllersList.size();
		if (newCameraIndex != currentCameraIndex) {
			const auto prevCamera = cameraControllersList[currentCameraIndex];
			const auto newCamera = cameraControllersList[newCameraIndex];
			currentCameraIndex = newCameraIndex;

			newCamera->setPosition(prevCamera->getPosition());
			newCamera->setDirection(prevCamera->getDirection());

			enableCamera(newCamera);
		}
	}

	// Update current camera state
	const auto currCamera = cameraControllersList[currentCameraIndex];
	currCamera->updatePerFrame();
}

void CameraManager::process()
{
}

void CameraManager::render()
{
}

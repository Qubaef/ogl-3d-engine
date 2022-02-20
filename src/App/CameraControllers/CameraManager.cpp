#include "CameraManager.h"

#include "Engine/Engine.h"
#include "App/Renderables/GuiEntityManager/Messages/RegisterEntityMessage.h"
#include "App/Renderables/GuiEntityManager/Messages/RegisterPropertyMessage.h"
#include "App/Renderables/GuiEntityManager/EntityProperties/IntPropertyWatcher.h"
#include "App/Renderables/GuiEntityManager/EntityProperties/Vec3PropertyWatcher.h"

#include "CameraController.h"
#include "InputManager.h"


CameraManager::CameraManager(Engine& engine, InputManager* inputManagerPtr) :
	IProcessable(engine), IMessanger(&engine.getMessageBus(), "CameraManager"),
	inputManagerPtr(inputManagerPtr)
{
	sendMessage(new RegisterEntityMessage(""), "EntityManager");

	inputManagerPtr->register_key_event(CAMERA_TOGGLE_KEY);

	sendMessage(new RegisterPropertyMessage("CameraManager",
		new IntPropertyWatcher("Camera index", currentCameraIndex)),
		"EntityManager");

	sendMessage(new RegisterPropertyMessage("CameraManager",
		new Vec3PropertyWatcher("Camera position", cameraPosition)),
		"EntityManager");

	sendMessage(new RegisterPropertyMessage("CameraManager",
		new Vec3PropertyWatcher("Camera direction", cameraDirection)),
		"EntityManager");
}

void CameraManager::enableCamera(CameraController* cameraControllerPtr)
{
	cameraControllerPtr->enable();
	engine.registerCamera(cameraControllerPtr);
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
	if (inputManagerPtr->getKeyState(CAMERA_TOGGLE_KEY) == KeyState::JUST_PRESSED)
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

	cameraPosition = cameraControllersList[currentCameraIndex]->getPosition();
	cameraDirection = cameraControllersList[currentCameraIndex]->getDirection();
}

void CameraManager::process()
{
}

void CameraManager::render()
{
}

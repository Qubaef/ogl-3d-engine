#pragma once

#include "CameraController.h"

class MenuUseCameraController : public CameraController
{
public:
	MenuUseCameraController(Engine& engine, InputManager* p_input_manager);
	void registerDefaultInput() override;
	void updatePerFrame() override;
	void enable() override;
};

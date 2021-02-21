#pragma once
#include "Includes.h"
#include "CameraController.h"
#include "Shader.h"

constexpr auto SKYBOX_DATA_PATH = "Data/Skybox";

class Skybox
{
	std::vector<std::string> names;
	Shader* p_shader;
	CameraController* p_camera_controller;

	GLuint VAO_id;

	void initialize_names();
	void load_skybox();

public:
	Skybox(std::string name, Shader* p_shader, CameraController* p_camera_controller);
	void render();
};

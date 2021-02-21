#include "Skybox.h"

Skybox::Skybox(std::string name, Shader* p_shader, CameraController* p_camera_controller)
{
	initialize_names();
	load_skybox();
}


void Skybox::initialize_names()
{
	names = {
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};
}


void Skybox::load_skybox()
{

}


void Skybox::render()
{
	glDepthMask(GL_FALSE);
	p_shader->use();

	
}

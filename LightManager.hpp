#pragma once
#include "LightDirectional.hpp"
#include "LightPoint.hpp"
#include "Shader.hpp"

class LightManager
{
	// Variable names connected with fragment shader names
	const std::string directional_light_name = "dir_light";
	const std::string point_lights_name = "point_lights";
	const std::string point_lights_count_name = "point_lights_count";

	std::vector<Shader*> shaders_vector;

	int point_lights_count = 0;
	LightPoint* point_lights[MAX_POINT_LIGHTS_COUNT] = { NULL };
	LightDirectional* directional_light;

	// function to define all lights present in the scene
	void initialize_scene_lights();

	// functions to update lights int the shader uniform
	void update_directional_light(Shader* p_shader) const;
	void update_point_lights(Shader* p_shader) const;

public:
	LightManager(std::vector<Shader*> shaders_vector);
	void update();
};

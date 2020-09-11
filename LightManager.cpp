#include "LightManager.hpp"

void LightManager::initialize_scene_lights()
{
	// initialize directional ligth
	directional_light = new LightDirectional(
		vec3(0.3f, 0.3f, 0.3f),
		vec3(0.7f, 0.7f, 0.7f),
		vec3(1.f, 1.f, 1.f),
		vec3(10.f, -1.f, 10.f));

	//// Initialize point lights
	// point_lights[0] = new LightPoint(
	// 	vec3(0.2f, 0.2f, 0.2f),
	// 	vec3(0.5f, 0.5f, 0.5f),
	// 	vec3(1.f, 1.f, 1.f),
	// 	vec3(5.f, 2.f, 5.f),
	// 	0.5f,
	// 	0.09,
	// 	0.032
	// );
	// 
	// point_lights[1] = new LightPoint(
	// 	vec3(0.2f, 0.2f, 0.2f),
	// 	vec3(0.5f, 0.5f, 0.5f),
	// 	vec3(1.f, 1.f, 1.f),
	// 	vec3(64.f, 2.f, 64.f),
	// 	1.0f,
	// 	0.09,
	// 	0.032
	// );

	// check how many point lights were defined
	for (int i = 0; i < MAX_POINT_LIGHTS_COUNT; i++)
	{
		if (point_lights[i] != NULL)
		{
			point_lights_count++;
		}
		else
		{
			break;
		}
	}
}


LightManager::LightManager(std::vector<Shader*> shaders_vector)
{
	this->shaders_vector = shaders_vector;
	initialize_scene_lights();

	// Init unchangeable uniforms
	for (Shader* p_shader : shaders_vector)
	{
		p_shader->use();
		p_shader->set_int(point_lights_count_name, point_lights_count);
	}

}


void LightManager::update()
{
	//// Define Animations for specific lights in the scene

	// Changing direction of directional light to simulate sunlight
	// mat4 rotation_matrix = rotate(mat4(1.0), radians(0.1f), vec3(1, 0, 0));
	// directional_light->set_direction(mat3(rotation_matrix) * directional_light->get_direction());

	//// Update lights to the uniforms
	for (Shader* p_shader : shaders_vector)
	{
		p_shader->use();
		update_directional_light(p_shader);
		update_point_lights(p_shader);
	}
}
void LightManager::update_directional_light(Shader* p_shader) const
{
	p_shader->set_vec3(directional_light_name + ".ambient", directional_light->get_ambient());
	p_shader->set_vec3(directional_light_name + ".diffuse", directional_light->get_diffuse());
	p_shader->set_vec3(directional_light_name + ".specular", directional_light->get_specular());
	p_shader->set_vec3(directional_light_name + ".direction", directional_light->get_direction());
}


void LightManager::update_point_lights(Shader* p_shader) const
{
	for (int i = 0; i < point_lights_count; i++)
	{
		std::string current_name = point_lights_name + '[' + static_cast<char>(i + 48) + ']';

		p_shader->set_vec3(current_name + ".ambient", point_lights[i]->get_ambient());
		p_shader->set_vec3(current_name + ".diffuse", point_lights[i]->get_diffuse());
		p_shader->set_vec3(current_name + ".specular", point_lights[i]->get_specular());
		p_shader->set_vec3(current_name + ".position", point_lights[i]->get_position());
		p_shader->set_float(current_name + ".constant", point_lights[i]->get_constant());
		p_shader->set_float(current_name + ".linear", point_lights[i]->get_linear());
		p_shader->set_float(current_name + ".quadratic", point_lights[i]->get_quadratic());
	}
}
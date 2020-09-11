#pragma once
#include "SimpleTerrain.hpp"

class WaterTerrain : public SimpleTerrain
{
	vec4 wave1;
	vec4 wave2;
	vec4 wave3;

	void generate_terrain() override;
	void prepare_data() override;
	void set_material() override;

public:
	WaterTerrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number, Shader* p_shader, CameraController* p_camera_controller);

	void render_terrain() override;
};

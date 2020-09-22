#pragma once
#include "SimpleTerrain.h"

class RandomTerrain : public SimpleTerrain
{
	void generate_terrain() override;
	void set_material() override;
	
public:
	RandomTerrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number, Shader* p_shader, CameraController* p_camera_controller);

	void render_terrain() override;
};

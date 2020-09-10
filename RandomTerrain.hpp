#pragma once
#include "SimpleTerrain.hpp"

class RandomTerrain : public SimpleTerrain
{
	void generate_terrain() override;

public:
	RandomTerrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number);

	void render_terrain() override;
};

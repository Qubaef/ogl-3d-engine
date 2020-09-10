#pragma once
#include "SimpleTerrain.hpp"

class WaterTerrain : public SimpleTerrain
{
	float time;
	vec4 wave1;
	vec4 wave2;
	vec4 wave3;

	void generate_terrain() override;
	void prepare_data() override;

	vec3 gerstner_wave(vec4 wave, vec3 p);

public:
	WaterTerrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number);

	void render_terrain() override;
};

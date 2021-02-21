#pragma once
#include "SimpleTerrain.h"
#include "SimplexNoise.h"

class SimplexTerrainChunk : public SimpleTerrain
{
	void generate_terrain() override;
	void set_material() override;

	float sediment_collected = 0;
	float sediment_deposited = 0;

	// TODO: move noise generation to wrapper class, as it will be probably used in other terrain classes
	// parameters defining noise
	const float amplitude = 150.0;
	const float frequency = 0.0018;
	const float lacunarity = 3.0;
	const float persistance = 0.25;
	const int terrain_layers = 5;

	const float noise_bias_x = 50;
	const float noise_bias_z = 50;

	// TODO: move erosion logic to separate class
	// Parameters defining erosion
	FILE* heightmap_file;
	std::string filename = "Heightmap-log";
	
	// number of generated droplets
	const int erosion_droplets_number = 50000;

	// max number of droplet jumps
	const int erosion_droplet_lifetime = 64;

	// initial droplet velocity and water
	const float erosion_droplet_init_velocity = 1;
	const float erosion_droplet_init_water = 1;

	// max capacity of the droplet (the higher it is, the steeper the terrain)
	const int erosion_droplet_capacity = 8;
	// inertia of the droplet (0,1) (the closer it is to 0, the more valleys and ravines will it create)
	const float erosion_droplet_inertia = 0.03;
	// deposition speed of the droplet (0,1)
	const float erosion_droplet_dspeed = 0.2;
	// erosion speed of the droplet (0,1)
	const float erosion_droplet_espeed = 0.7;
	// evaporation speed of the droplet (0,1)
	const float erosion_droplet_evaporation = 0.02;
	// erosion radius
	const int erosion_droplet_radius = 4;
	// minimal slope of erosion
	const float erosion_minimal_slope = 0.01;
	// erosion gravity
	const float erosion_gravity = 10;


	// Erosion methods:

	void perform_erosion();
	void simulate_droplet(int &skipped, int& stillsame);

	// calculate gradient vector basing on points on mesh, basing on given position
	vec2 calculate_gradient(vec2 position);

	// calculate interpolated height value for given position
	float calculate_height(vec2 position);

	// deposit sediment for given position
	void sediment_deposit(vec2 position, float amount);

	// perform erosion for given position
	float sediment_erosion(vec2 position, float amount);

	// calculate index of closest top-right vertex
	int calculate_index(vec2 position);


public:
	SimplexTerrainChunk(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number, Shader* p_shader, CameraController* p_camera_controller);

	void render_terrain() override;
};
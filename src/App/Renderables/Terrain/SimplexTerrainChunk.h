#pragma once
#include <vector>

#include "Engine/Common/Vao.h"
#include "Engine/Common/Vbo.h"

#include "Terrain.h"
#include "SimplexNoiseWrapper.h"

class SimplexTerrainChunk : public Terrain
{

	Shader* shaderPtr;
	int sectorSizeX;
	int sectorSizeY;
	int sectorsNumberX;
	int sectorsNumberY;

	int pointsPerSectorLine;

	Vao mainVao;
	Vbo vertexBuffer;
	Vbo normalsBuffer;
	Vbo indicesBuffer;

	glm::vec3* vertexData;
	glm::vec3* normalsData;
	std::vector<int> indicesData;

	float sediment_collected = 0;
	float sediment_deposited = 0;

	SimplexNoiseWrapper simplexNoiseParams;

	// TODO: move erosion logic to separate class
	// Parameters defining erosion
	FILE* heightmap_file;
	std::string filename = "Heightmap-log";

	// number of generated droplets
	const int erosion_droplets_number = 70;

	// max number of droplet jumps
	const int erosion_droplet_lifetime = 64;

	// initial droplet velocity and water
	const float erosion_droplet_init_velocity = 1.f;
	const float erosion_droplet_init_water = 1.f;

	// max capacity of the droplet (the higher it is, the steeper the terrain)
	const int erosion_droplet_capacity = 8;
	// inertia of the droplet (0,1) (the closer it is to 0, the more valleys and ravines will it create)
	const float erosion_droplet_inertia = 0.03f;
	// deposition speed of the droplet (0,1)
	const float erosion_droplet_dspeed = 0.2f;
	// erosion speed of the droplet (0,1)
	const float erosion_droplet_espeed = 0.7f;
	// evaporation speed of the droplet (0,1)
	const float erosion_droplet_evaporation = 0.02f;
	// erosion radius
	const int erosion_droplet_radius = 4;
	// minimal slope of erosion
	const float erosion_minimal_slope = 0.01f;
	// erosion gravity
	const float erosion_gravity = 10.f;

	// Update heightmap according to SimplexNoiseParams
	void updateHeightmap();

	/* Erosion methods: */
	void perform_erosion();
	void simulate_droplet(int& skipped, int& stillsame);

	// calculate gradient vector basing on points on mesh, basing on given position
	glm::vec2 calculate_gradient(glm::vec2 position);

	// calculate interpolated height value for given position
	float calculate_height(glm::vec2 position);

	// deposit sediment for given position
	void sediment_deposit(glm::vec2 position, float amount);

	// perform erosion for given position
	float sediment_erosion(glm::vec2 position, float amount);

	// calculate index of closest top-right vertex
	int calculate_index(glm::vec2 position);


public:
	SimplexTerrainChunk(int sectorSizeX, int sectorSizeY, int sectorsNumberX, int sectorsNumberY, int pointsPerSectorLine, Shader* shaderPtr);

	void initialize() override;

	void update() override;

	void sendAndRender() override;

	Shader* getShader() override;

	~SimplexTerrainChunk() override;

	void updateSimplexNoise(SimplexNoiseWrapper simplexNoiseParams);

	SimplexNoiseWrapper getSimplexNoise();
};

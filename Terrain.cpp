#include "Terrain.hpp"

Terrain::Terrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number)
	:
	start_pos_x(start_pos_x),
	start_pos_z(start_pos_z),
	terrain_size(terrain_size),
	vertices_number(vertices_number)
{
}


Terrain::~Terrain()
{
}

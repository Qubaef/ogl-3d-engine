#pragma once
#include "Quad.h"

class QuadTreeLOD
{
	vec3* mesh_array;
	int mesh_size;

	float lod_level;

public:
	QuadTreeLOD(vec3* mesh_array, int mesh_size, float lod_level);

	// Create mesh with level of detail level
	void create_lod_mesh(std::vector<unsigned> &indices, vec3 camera_position);
	void quad_step(std::vector<unsigned>& indices, vec3& camera_position, int tl, int tr, int bl, int br);
};

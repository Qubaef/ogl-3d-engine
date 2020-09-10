#pragma once
#include "Terrain.hpp"

class SimpleTerrain : public Terrain
{
protected:
	// Ids of OGL objects
	GLuint vertices_VBO_id;
	unsigned indices_VBO_id;
	GLuint normals_VBO_id;

	// Array storing points data
	vec3* vertex_data;

	// Vector storing indices data
	std::vector<unsigned> indices_data;

	// Array storing normals data
	vec3* normals_data;

	SimpleTerrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number);

	void init_indices_data();
	void init_normals_data();

	void initialize_OGL_objects() override;
	void prepare_data() override;

public:
	~SimpleTerrain() override;
};

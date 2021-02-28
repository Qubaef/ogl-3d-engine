#pragma once
#include "_Terrain.h"
#include "QuadTreeLOD.h"

class DynamicTerrain : public _Terrain
{
	const float lod_level = SECTOR_SIZE / 2;
	const float max_lod_depth = 0;
	QuadTreeLOD* p_quad_tree_lod;

	GLuint vertices_VBO_id;
	unsigned indices_VBO_id;
	GLuint normals_VBO_id;

	// Array storing points data
	vec3* vertex_data;

	// Vector storing indices data
	std::vector<unsigned> indices_data;

	// Array storing normals data
	vec3* normals_data;

	// previous camera position (used to save up calculations while idle)
	vec3 prev_camera_position;

	// TODO: these methods are temporary
	void init_indices_data();
	void init_normals_data();

	// Cache all terrain mesh data to huge buffer
	void generate_terrain() override;

	// Set material which will be used in the shader
	void set_material();

	// Initialize all used OGL buffers
	void initialize_OGL_objects() override;

	// Send proper data to the buffer
	void prepare_data() override;

public:
	DynamicTerrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number, Shader* p_shader, CameraController* p_camera_controller);
	~DynamicTerrain() override;

	// Render frame of terrain (has to be fast af)
	void render_terrain() override;
};

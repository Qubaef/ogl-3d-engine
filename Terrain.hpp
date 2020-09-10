#pragma once
#include "Includes.hpp"

class Terrain
{
protected:
	GLuint main_VAO_id;

	const int start_pos_x;
	const int start_pos_z;

	const int terrain_size;

	const int vertices_number;

	Terrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number);

	// Each Terrain should provide it's own implementation of initialize_OGL_objects method
	// initialize_OGL_objects should be called in the constructor, and it should initialize all used ogl objects
	virtual void initialize_OGL_objects() = 0;

	// Each Terrain should provide it's own implementation of generate_terrain method
	// generate_terrain should be called in the constructor, and it should initialize height_array
	virtual void generate_terrain() = 0;

	// Each Terrain should provide it's own implementation of prepare_data method
	// prepare_data should format and send all data to gpu
	// depending on terrain implementation, it should be sent just once in generate_terrain() (static terrain)
	// or should be sent every frame in render_terrain (dynamic terrain)
	virtual void prepare_data() = 0;
public:
	// Each Terrain should provide it's own implementation of render_terrain method
	// render_terrain will be called each frame, so it should update ogl buffers and call draw functions for them
	virtual void render_terrain() = 0;

	virtual ~Terrain();
};

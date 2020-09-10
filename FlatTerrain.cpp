#include "FlatTerrain.hpp"

FlatTerrain::FlatTerrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number)
	: SimpleTerrain(start_pos_x, start_pos_z, terrain_size, vertices_number)
{
	generate_terrain();
}


void FlatTerrain::generate_terrain()
{
	// init vertex data
	vertex_data = new vec3[vertices_number * vertices_number];

	for (int i = 0; i < vertices_number; i++)
	{
		for (int j = 0; j < vertices_number; j++)
		{
			vertex_data[(j + i * (SECTOR_DENSITY))] = vec3(i * (float)SECTOR_SIZE / SECTOR_DENSITY, 0, j * (float)SECTOR_SIZE / SECTOR_DENSITY);
		}
	}

	// init indices data
	init_indices_data();

	// init normals data
	init_normals_data();

	// call prepare_data(), as FlatTerrain is static
	prepare_data();
}


void FlatTerrain::render_terrain()
{
	// Bind to VAO to perform draw operation
	glBindVertexArray(main_VAO_id);

	glDrawElements(
		GL_TRIANGLES,			// mode
		indices_data.size(),	// count
		GL_UNSIGNED_INT,		// type
		(void*)0				// element array buffer offset
	);
}
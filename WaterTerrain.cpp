#include "WaterTerrain.hpp"

WaterTerrain::WaterTerrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number, Shader* p_shader, CameraController* p_camera_controller)
	: SimpleTerrain(start_pos_x, start_pos_z, terrain_size, vertices_number, p_shader, p_camera_controller)
{
	// wave parameters (direction), steepness, wavelength
	wave1 = vec4(normalize(vec2(1, 1)), 0.25, 60);
	wave2 = vec4(normalize(vec2(1, 0.6)), 0.25, 31);
	wave3 = vec4(normalize(vec2(1, 1.3)), 0.25, 18);

	generate_terrain();
}


void WaterTerrain::generate_terrain()
{
	// init vertex data
	vertex_data = new vec3[vertices_number * vertices_number];

	// calculate vertex data
	for (int i = 0; i < vertices_number; i++)
	{
		for (int j = 0; j < vertices_number; j++)
		{
			vertex_data[(j + i * (vertices_number))] = vec3(i * (float)SECTOR_SIZE / vertices_number, 0, j * (float)SECTOR_SIZE / vertices_number);
		}
	}

	// init indices data
	init_indices_data();

	// init normals data
	init_normals_data();

	// call prepare_data(), as FlatTerrain is static
	prepare_data();
}


void WaterTerrain::prepare_data()
{
	// Attributes setup and global VAO creation

	// bind global VAO object
	glBindVertexArray(main_VAO_id);
	p_shader->use();

	// select indices VBO (to perform VBO indexing)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_VBO_id);
	// bind indices VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_data.size() * sizeof(unsigned int), &indices_data[0], GL_DYNAMIC_DRAW);

	//// 1st vertex shader input attribute - vertex data
	// select vertex VBO
	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO_id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices_number * vertices_number, vertex_data, GL_STATIC_DRAW);
	// redirect buffer to input of the shader
	glVertexAttribPointer(
		0,						// location in shader
		3,						// size
		GL_FLOAT,				// type
		GL_FALSE,				// normalized
		0,						// stride (space between following fragments)
		(void*)0				// array buffer offset
	);
	// enable attribute '0'
	glEnableVertexAttribArray(0);

	// Set uniforms
	p_shader->set_vec4("wave[0]", wave1);
	p_shader->set_vec4("wave[1]", wave2);
	p_shader->set_vec4("wave[2]", wave3);

	set_material();

	get_MVP_handles();
}


void WaterTerrain::set_material()
{
	// Set material
	p_shader->use();
	p_shader->set_vec3("material.ambient", vec3(0.f, 0.41f, 0.58f));
	p_shader->set_vec3("material.diffuse", vec3(0.f, 0.61f, 0.78f));
	p_shader->set_vec3("material.specular", vec3(0.6f, 0.6f, 0.6f));
	p_shader->set_float("material.shininess", 128);
}


void WaterTerrain::render_terrain()
{
	// call universal render_terrain() method
	Terrain::render_terrain();

	// Bind to VAO to perform draw operation
	glBindVertexArray(main_VAO_id);

	// Send view position for specular component (TODO: check if can be done better)
	p_shader->set_vec3("view_pos", p_camera_controller->getPosition());
	p_shader->set_float("time", glfwGetTime());

	glDrawElements(
		GL_TRIANGLES,			// mode
		indices_data.size(),	// count
		GL_UNSIGNED_INT,		// type
		(void*)0				// element array buffer offset
	);
}
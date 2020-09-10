#include "WaterTerrain.hpp"

WaterTerrain::WaterTerrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number)
	: SimpleTerrain(start_pos_x, start_pos_z, terrain_size, vertices_number)
{
	time = 0;

	// wave parameters (direction), steepness, wavelength
	wave1 = vec4(normalize(vec2(1, 0)), 0.25, 60);
	wave2 = vec4(normalize(vec2(0, 0.6)), 0.25, 31);
	wave3 = vec4(normalize(vec2(1, 1.3)), 0.25, 18);

	generate_terrain();
}


void WaterTerrain::generate_terrain()
{
	// init vertex data
	vertex_data = new vec3[vertices_number * vertices_number];

	// init normals data
	normals_data = new vec3[vertices_number * vertices_number];

	// init indices data and send it to the buffer, and this data is static
	init_indices_data();

	// bind global VAO object
	glBindVertexArray(main_VAO_id);

	// select indices VBO (to perform VBO indexing)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_VBO_id);
	// bind indices VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_data.size() * sizeof(unsigned int), &indices_data[0], GL_DYNAMIC_DRAW);
}


void WaterTerrain::prepare_data()
{
	time += 0.1;

	// calculate vertex data
	for (int i = 0; i < vertices_number; i++)
	{
		for (int j = 0; j < vertices_number; j++)
		{
			vec3 vertex = vec3(i * (float)SECTOR_SIZE / vertices_number, 0, j * (float)SECTOR_SIZE / vertices_number);
			vertex_data[(j + i * (vertices_number))] = gerstner_wave(wave1, vertex);
			vertex_data[(j + i * (vertices_number))] += gerstner_wave(wave2, vertex);
			vertex_data[(j + i * (vertices_number))] += gerstner_wave(wave3, vertex);
		}
	}

	// calculate normals data
	for (int i = 0; i < vertices_number; i++)
	{
		for (int j = 0; j < vertices_number; j++)
		{
			vec3 current = vertex_data[(j + i * (vertices_number))];

			// init normal vector
			vec3 normal = vec3(0, 0, 0);

			if (i > 0 && j < vertices_number - 1)
			{
				normal += cross((vertex_data[((j + 1) + (i - 1) * (vertices_number))] - current), (current - vertex_data[(j + (i - 1) * (vertices_number))]));
			}

			if (j > 0 && i < SECTOR_DENSITY - 1)
			{
				normal += cross((current - vertex_data[((j - 1) + i * (vertices_number))]), (vertex_data[((j - 1) + (i + 1) * (vertices_number))] - current));
			}

			if (i < SECTOR_DENSITY - 1 && j < SECTOR_DENSITY - 1)
			{
				normal += cross((current - vertex_data[(j + (i + 1) * (vertices_number))]), (vertex_data[((j + 1) + i * (vertices_number))] - current));
			}

			// normalize vector's length
			if (length(normal) == 0)
			{
				normal.y = 1;
			}

			normals_data[(j + i * (vertices_number))] = normalize(normal);
		}
	}

	// bind global VAO object
	glBindVertexArray(main_VAO_id);

	//// 1st vertex shader input attribute - vertex data
	// select vertex VBO
	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO_id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices_number * vertices_number, vertex_data, GL_DYNAMIC_DRAW);
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

	// 2nd vertex shader input attribute - normals
	// select normals VBO
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO_id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices_number * vertices_number, normals_data, GL_DYNAMIC_DRAW);
	// redirect buffer to input of the shader
	glVertexAttribPointer(
		1,                      // location in shader
		3,                      // size
		GL_FLOAT,               // type
		GL_FALSE,               // normalized
		0,                      // stride (space between following fragments)
		(void*)0                // array buffer offset
	);
	// enable attribute '1'
	glEnableVertexAttribArray(1);
}


vec3 WaterTerrain::gerstner_wave(vec4 wave, vec3 p)
{
	float k = 2 * glm::pi<float>() / wave.w;
	float c = sqrt(9.8 / k);
	vec2 d = vec2(wave.x, wave.y);
	float f = k * (dot(d, vec2(p.x, p.z)) - c * time);
	float a = wave.z / k;
	float cosf = cos(f);

	return vec3(
		p.x + d.x * (a *cosf),
		a * sin(f),
		p.z + d.y * (a *cosf));
}


void WaterTerrain::render_terrain()
{
	// call prepare_data(), as 
	prepare_data();

	glDrawElements(
		GL_TRIANGLES,			// mode
		indices_data.size(),	// count
		GL_UNSIGNED_INT,		// type
		(void*)0				// element array buffer offset
	);
}
#include "SimplexTerrainChunk.h"

SimplexTerrainChunk::SimplexTerrainChunk(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number, Shader* p_shader, CameraController* p_camera_controller)
	: SimpleTerrain(start_pos_x, start_pos_z, terrain_size, vertices_number, p_shader, p_camera_controller)
{
	generate_terrain();
	set_material();
}


void SimplexTerrainChunk::generate_terrain()
{
	// init vertex data
	vertex_data = new vec3[vertices_number * vertices_number];

	for (int i = 0; i < vertices_number; i++)
	{
		for (int j = 0; j < vertices_number; j++)
		{
			vec3 vertex = vec3(start_pos_x + i * (float)terrain_size / vertices_number, 0, start_pos_z + j * (float)terrain_size / vertices_number);
			vertex.y = amplitude + amplitude * SimplexNoise(frequency, amplitude, lacunarity, persistance).fractal(terrain_layers, noise_bias_x + vertex.x, noise_bias_z + vertex.z);
			vertex_data[(j + i * (vertices_number))] = vertex;
		}
	}

	// vertex_data[0] = vec3(0, 0, 0);
	// vertex_data[1] = vec3(0, 0.5, 1);
	// vertex_data[2] = vec3(1, 0.5, 0);
	// vertex_data[3] = vec3(1, 1, 1);
	// 
	// vec2 testG = calculate_gradient(vec2(0.5, 0.5));
	// 
	// testG.x /= terrain_size / vertices_number * sqrt(2);
	// testG.y /= terrain_size / vertices_number * sqrt(2);
	// float test_length = length(testG);

	// perform erosion on generated terrain
	perform_erosion();

	// init indices data
	init_indices_data();

	// init normals data
	init_normals_data();

	// call prepare_data(), as FlatTerrain is static
	prepare_data();
}


void SimplexTerrainChunk::perform_erosion()
{
	int skipped = 0;
	int stillsame = 0;

	fclose(fopen(filename.c_str(), "w"));
	heightmap_file = fopen(filename.c_str(), "a+");

	fprintf(heightmap_file, "x higher to bottom\ny higher to the right\n");
	
	// simulate droplets sequentially
	for (int point = 0; point < erosion_droplets_number; point++)
	{
		fprintf(heightmap_file, "Droplet nr %d \n", point);
		
		if(point == 18)
		{
			__asm nop;
		}

		simulate_droplet(skipped, stillsame);
	}

	printf("collected: %f deposited: %f\n", sediment_collected, sediment_deposited);
	printf("skipped %d still in the same %d\n", skipped, stillsame);

	fclose(heightmap_file);
}


void SimplexTerrainChunk::simulate_droplet(int& skipped, int& stillsame)
{
	// randomize droplet position and set its values
	vec2 pos(start_pos_x + (float)(std::rand() - 1) / RAND_MAX * (terrain_size - 1), start_pos_z + (float)(std::rand() - 1) / RAND_MAX * (terrain_size - 1));

	// fprintf(heightmap_file, "Initial pos: %.2f %.2f = %d\n", pos.x, pos.y, calculate_index(pos));

	vec2 droplet_dir(0, 0);
	vec2 tmp_prev_pos;

	float height = calculate_height(pos);
	float velocity = erosion_droplet_init_velocity;
	float water = erosion_droplet_init_water;
	float sediment = 0;
	// float normalization_factor = terrain_size / vertices_number * sqrt(2);

	// simulate droplet flow and update heights
	for (int droplet_lifetime = 0; droplet_lifetime < erosion_droplet_lifetime; droplet_lifetime++)
	{
		// perform bilinear interpolation for current position
		vec2 g = calculate_gradient(pos);

		// from gradient, calculate new direction vector
		droplet_dir = droplet_dir * erosion_droplet_inertia - g * (1 - erosion_droplet_inertia);

		// if droplet is on flat tile, randomize the direction
		if (length(droplet_dir) == 0)
		{
			droplet_dir.x = (float)(std::rand()) / RAND_MAX * 2 - 1;
			droplet_dir.y = (float)(std::rand()) / RAND_MAX * 2 - 1;
		}

		// normalize droplet_dir to the length, that will move it to next tile
		droplet_dir = normalize(droplet_dir);
		droplet_dir *= TILE_SIZE * sqrt(2);

		// calculate new position according the direction vector (move droplet)
		tmp_prev_pos = pos;
		pos = pos + droplet_dir;
		// fprintf(heightmap_file, "New pos: %.2f %.2f = %d\n", pos.x, pos.y, calculate_index(pos));

		// check if droplet move was valid
		vec2 tmp_pos_diff = abs((tmp_prev_pos - pos));

		if (pos.x < vertex_data[calculate_index(tmp_prev_pos)].x - TILE_SIZE ||
			pos.x > vertex_data[calculate_index(tmp_prev_pos)].x + 2 * TILE_SIZE ||
			pos.y < vertex_data[calculate_index(tmp_prev_pos)].z - TILE_SIZE ||
			pos.y > vertex_data[calculate_index(tmp_prev_pos)].z + 2 * TILE_SIZE)
		{
			//printf("Skipped a tile!\n");
			skipped++;
		}
		else if (pos.x > vertex_data[calculate_index(tmp_prev_pos)].x &&
			pos.x < vertex_data[calculate_index(tmp_prev_pos)].x + TILE_SIZE &&
			pos.y > vertex_data[calculate_index(tmp_prev_pos)].z &&
			pos.y < vertex_data[calculate_index(tmp_prev_pos)].z + TILE_SIZE)
		{
			//printf("Still in the same tile!\n");
			stillsame++;
		}

		// TODO: maybe check if new pos is in the neighboring tile

		// stop if droplet left the map
		if (pos.x < start_pos_x || pos.y < start_pos_z || pos.x >= start_pos_x + terrain_size - 1 || pos.y >= start_pos_z + terrain_size - 1)
		{
			break;
		}

		// calculate height difference covered by the moving droplet
		float height_new = calculate_height(pos);
		float h_dif = height_new - height;
		height = height_new;

		// calculate capacity carried by the droplet
		float c = max(-h_dif, erosion_minimal_slope) * velocity * water * erosion_droplet_capacity;

		// if sediment is higher than capacity, or if droplet is flowing uphill
		if (sediment > c || h_dif > 0)
		{
			// If true, droplet deposits collected sediment to nearby vertices

			// Calculate amount to deposit:
			// * if droplet travels uphill, it should deposit all collected sediment (but not to make terrain higher than previous node
			//	 to prevent spikes)
			// * if sediment is higher than capacity, it should only deposit certain fraction of stored sediment
			float amount_to_deposit = h_dif > 0 ? min(h_dif, sediment) : min((sediment - c) * erosion_droplet_dspeed, -h_dif);
			sediment -= amount_to_deposit;

			if (sediment < 0)
			{
				printf("dropped to much sediment\n");
			}

			sediment_deposited += amount_to_deposit;

			if (amount_to_deposit < 0)
			{
				printf("Deposit amount less than 0\n");
			}

			// deposit calculated amount in nearby points
			sediment_deposit(tmp_prev_pos, amount_to_deposit);
		}
		else
		{
			// If false, droplet erodes surrounding vertices (which amount is determined by radius)
			float amount_to_erode = min((c - sediment) * erosion_droplet_espeed, -h_dif);

			// erode surrounding vertices and collect sediment from them
			float amount_collected = sediment_erosion(pos, amount_to_erode);
			sediment += amount_collected;
			sediment_collected += amount_collected;
		}

		float velocity_cofactor = velocity * velocity + h_dif * erosion_gravity;
		if (velocity_cofactor > 0)
		{
			velocity = sqrt(velocity_cofactor);
		}
		else
		{
			droplet_dir = -droplet_dir;
			velocity = sqrt(-velocity_cofactor);
		}

		// calculate new velocity
		velocity = sqrt(abs(velocity * velocity + h_dif * erosion_gravity));

		// calculate new water amount
		water = water * (1 - erosion_droplet_evaporation);

		if (velocity != velocity)
		{
			printf("kurwa");
		}
	}

	// Print terrain height for test purposes

	// for (int i = 0; i < vertices_number; i++)
	// {
	// 	for (int j = 0; j < vertices_number; j++)
	// 	{
	// 		fprintf(heightmap_file, "%.2f ", vertex_data[(j + i * (vertices_number))].y);
	// 	}
	// 
	// 	fprintf(heightmap_file, "\n");
	// }
	// 
	// fprintf(heightmap_file, "\n\n\n");
}



vec2 SimplexTerrainChunk::calculate_gradient(vec2 position)
{
	// calculate index of vertex which is closest top-left point in the mesh
	int index = calculate_index(position);

	float h = vertex_data[index].y; // NW
	float h_x = vertex_data[index + vertices_number].y; // SW
	float h_z = vertex_data[index + 1].y; // NE
	float h_xz = vertex_data[index + vertices_number + 1].y; // SE

	// calculate u and v, which are offsets
	float u = position.x - vertex_data[index].x; // x
	float v = position.y - vertex_data[index].z; // y

	// calculate gradient
	float x_factor = (h_x - h) * (1 - v) + (h_xz - h_z) * v;
	float z_factor = (h_z - h) * (1 - u) + (h_xz - h_x) * u;

	// generate vector and normalize it, to be 'one tile' long
	vec2 g(x_factor, z_factor);
	// return normalize(g) * ((float)terrain_size / vertices_number);
	 return g;
}


float SimplexTerrainChunk::calculate_height(vec2 position)
{
	int index = calculate_index(position);

	float x1 = vertex_data[index].x;
	float x2 = vertex_data[index + vertices_number].x;

	float z1 = vertex_data[index].z;
	float z2 = vertex_data[index + 1].z;

	// calculate two linear interpolations in x-direction 
	float x_dir_1 = (x2 - position.x) / (x2 - x1) * vertex_data[index].y + (position.x - x1) / (x2 - x1) * vertex_data[index + vertices_number].y;
	float x_dir_2 = (x2 - position.x) / (x2 - x1) * vertex_data[index + 1].y + (position.x - x1) / (x2 - x1) * vertex_data[index + vertices_number + 1].y;

	// calculate bilinear interpolation, using x-direction interpolations calculated above
	return (z2 - position.y) / (z2 - z1) * x_dir_1 + (position.y - z1) / (z2 - z1) * x_dir_2;
}


void SimplexTerrainChunk::sediment_deposit(vec2 position, float amount)
{
	int index = calculate_index(position);

	// calculate u and v, which are offsets
	float u = position.x - vertex_data[index].x;
	float v = position.y - vertex_data[index].z;

	// float tmp_amount = 0;
	// tmp_amount += amount * (1 - u) * (1 - v);
	// if (tmp_amount > amount)
	// {
	// 	printf("Too much\n");
	// }
	// vertex_data[index].y += amount * (1 - u) * (1 - v);
	// 
	// tmp_amount += amount * u * (1 - v);
	// if (tmp_amount > amount)
	// {
	// 	printf("Too much\n");
	// }
	// vertex_data[index + 1].y += amount * u * (1 - v);
	// 
	// tmp_amount += amount * (1 - u) * v;
	// if (tmp_amount > amount)
	// {
	// 	printf("Too much\n");
	// }
	// vertex_data[index + vertices_number].y += amount * (1 - u) * v;
	// 
	// tmp_amount += amount * u * v;
	// if (tmp_amount > amount + 0.0001)
	// {
	// 	printf("Too much\n");
	// }
	// vertex_data[index + vertices_number + 1].y += amount * u * v;

	vertex_data[index].y += amount * (1 - u) * (1 - v);
	vertex_data[index + 1].y += amount * u * (1 - v);
	vertex_data[index + vertices_number].y += amount * (1 - u) * v;
	vertex_data[index + vertices_number + 1].y += amount * u * v;
}


float SimplexTerrainChunk::sediment_erosion(vec2 position, float amount)
{
	int index = calculate_index(position);

	// calculate index of top-left vertex of the square, which will be used to calculate erosion
	int row = index / vertices_number;
	int col = index % vertices_number;

	int top_row = max(0, row - (erosion_droplet_radius - 1));
	int left_col = max(0, col - (erosion_droplet_radius - 1));

	int top_left_index = left_col + top_row * vertices_number;

	int x_radius = min(vertices_number - 1, col + erosion_droplet_radius) - left_col;
	int z_radius = min(vertices_number - 1, row + erosion_droplet_radius) - top_row;

	float weight_sum = 0;

	// calculate weights sum
	for (int i = 0; i < x_radius; i++)
	{
		for (int j = 0; j < z_radius; j++)
		{
			vec2 point_pos(vertex_data[top_left_index + i + (j * vertices_number)].x, vertex_data[top_left_index + i + (j * vertices_number)].z);
			weight_sum += max(0.f, (erosion_droplet_radius - distance(point_pos, position)));
		}
	}

	float sediment = 0;

	// perform erosion for each point
	for (int i = 0; i < x_radius; i++)
	{
		for (int j = 0; j < z_radius; j++)
		{
			vec2 point_pos(vertex_data[top_left_index + i + (j * vertices_number)].x, vertex_data[top_left_index + i + (j * vertices_number)].z);
			float weight = max(0.f, (erosion_droplet_radius - distance(point_pos, position)));

			float wighted_amount = amount * weight / weight_sum;

			// calculate delta sediment for terrain, not to drop below 0
			float height = vertex_data[top_left_index + i + (j * vertices_number)].y;
			float d_sediment = height < wighted_amount ? height : wighted_amount;

			// erode point
			vertex_data[top_left_index + i + (j * vertices_number)].y -= d_sediment;

			// gather collected sediment
			sediment += d_sediment;
		}
	}

	return sediment;
}



int SimplexTerrainChunk::calculate_index(vec2 position)
{
	// calculate x and y, which are indexes of closest top-left point in the mesh
	int x = (position.x - start_pos_x) / ((float)terrain_size / vertices_number);
	int z = (position.y - start_pos_z) / ((float)terrain_size / vertices_number);

	return z + x * (vertices_number);
}


void SimplexTerrainChunk::set_material()
{
	// Set material
	p_shader->use();
	p_shader->set_vec3("material.ambient", vec3(0.298f, 0.282f, 0.27f));
	p_shader->set_vec3("material.diffuse", vec3(0.458f, 0.411f, 0.341f));
	p_shader->set_vec3("material.colorSpecular", vec3(0.0f, 0.0f, 0.0f));
	p_shader->set_float("material.shininess", 128);
}


void SimplexTerrainChunk::render_terrain()
{
	// call universal render_terrain() method
	Terrain::render_terrain();

	// Bind to VAO to perform draw operation
	glBindVertexArray(main_VAO_id);

	// Send view position for colorSpecular component
	p_shader->set_vec3("view_pos", p_camera_controller->getPosition());

	glDrawElements(
		GL_TRIANGLES,			// mode
		indices_data.size(),	// count
		GL_UNSIGNED_INT,		// type
		(void*)0				// element array buffer offset
	);
}
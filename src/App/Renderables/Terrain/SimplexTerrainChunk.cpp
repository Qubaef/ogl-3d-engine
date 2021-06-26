#include "SimplexTerrainChunk.h"
#include "Engine/Include/Constants.h"

SimplexTerrainChunk::SimplexTerrainChunk(int sectorSizeX, int sectorSizeY, int sectorsNumberX, int sectorsNumberY, int pointsPerSectorLine, Shader* shaderPtr)
	: shaderPtr(shaderPtr),
	sectorSizeX(sectorSizeX),
	sectorSizeY(sectorSizeY),
	sectorsNumberX(sectorsNumberX),
	sectorsNumberY(sectorsNumberY),
	pointsPerSectorLine(pointsPerSectorLine)
{
	ZoneScoped;

	this->shaderPtr->use();

	// Generate buffer for vertex data
	vertexData = new glm::vec3[pointsPerSectorLine * pointsPerSectorLine];

	// Generate buffer for indicesData
	indicesData.reserve((pointsPerSectorLine - 1) * (pointsPerSectorLine - 1) * 6);

	// Generate buffer for normalsData
	normalsData = new glm::vec3[pointsPerSectorLine * pointsPerSectorLine];
}

void SimplexTerrainChunk::updateHeightmap()
{
	// Update heightmap
	for (int i = 0; i < pointsPerSectorLine; i++)
	{
		for (int j = 0; j < pointsPerSectorLine; j++)
		{
			vertexData[(j + i * (pointsPerSectorLine))].y = simplexNoiseParams.getSimplexNoiseValue(i, j);
		}
	}

	// Init normals data
	for (int i = 0; i < pointsPerSectorLine; i++)
	{
		for (int j = 0; j < pointsPerSectorLine; j++)
		{
			glm::vec3 current = vertexData[(j + i * (pointsPerSectorLine))];

			// Init normal vector
			glm::vec3 normal = glm::vec3(0, 0, 0);

			if (i > 0 && j < pointsPerSectorLine - 1)
			{
				normal += glm::cross((vertexData[((j + 1) + (i - 1) * (pointsPerSectorLine))] - current), (current - vertexData[(j + (i - 1) * (pointsPerSectorLine))]));
			}

			if (j > 0 && i < pointsPerSectorLine - 1)
			{
				normal += glm::cross((current - vertexData[((j - 1) + i * (pointsPerSectorLine))]), (vertexData[((j - 1) + (i + 1) * (pointsPerSectorLine))] - current));
			}

			if (i < pointsPerSectorLine - 1 && j < pointsPerSectorLine - 1)
			{
				normal += glm::cross((current - vertexData[(j + (i + 1) * (pointsPerSectorLine))]), (vertexData[((j + 1) + i * (pointsPerSectorLine))] - current));
			}

			// Normalize vector's length
			if (length(normal) == 0)
			{
				normal.y = 1;
			}

			normalsData[(j + i * (pointsPerSectorLine))] = normalize(normal);
		}
	}
}

void SimplexTerrainChunk::perform_erosion()
{
	ZoneScoped;

	int skipped = 0;
	int stillsame = 0;

	// fclose(fopen(filename.c_str(), "w"));
	// heightmap_file = fopen(filename.c_str(), "a+");

	// fprintf(heightmap_file, "x higher to bottom\ny higher to the right\n");

	// simulate droplets sequentially
	for (int point = 0; point < erosion_droplets_number; point++)
	{
		// fprintf(heightmap_file, "Droplet nr %d \n", point);
		simulate_droplet(skipped, stillsame);
	}

	// printf("collected: %f deposited: %f\n", sediment_collected, sediment_deposited);
	// printf("skipped %d still in the same %d\n", skipped, stillsame);

	// fclose(heightmap_file);
}


void SimplexTerrainChunk::simulate_droplet(int& skipped, int& stillsame)
{
	ZoneScoped;

	// randomize droplet position and set its values
	vec2 pos(vertexData[0].x + (float)(std::rand() - 1) / RAND_MAX * (sectorSizeX - 1), vertexData[0].z + (float)(std::rand() - 1) / RAND_MAX * (sectorSizeY - 1));

	// fprintf(heightmap_file, "Initial pos: %.2f %.2f = %d\n", pos.x, pos.y, calculate_index(pos));

	vec2 droplet_dir(0, 0);
	vec2 tmp_prev_pos;

	float height = calculate_height(pos);
	float velocity = erosion_droplet_init_velocity;
	float water = erosion_droplet_init_water;
	float sediment = 0;
	// float normalization_factor = terrain_size / pointsPerSectorLine * sqrt(2);

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

		if (pos.x < vertexData[calculate_index(tmp_prev_pos)].x - TILE_SIZE ||
			pos.x > vertexData[calculate_index(tmp_prev_pos)].x + 2 * TILE_SIZE ||
			pos.y < vertexData[calculate_index(tmp_prev_pos)].z - TILE_SIZE ||
			pos.y > vertexData[calculate_index(tmp_prev_pos)].z + 2 * TILE_SIZE)
		{
			//printf("Skipped a tile!\n");
			skipped++;
		}
		else if (pos.x > vertexData[calculate_index(tmp_prev_pos)].x &&
			pos.x < vertexData[calculate_index(tmp_prev_pos)].x + TILE_SIZE &&
			pos.y > vertexData[calculate_index(tmp_prev_pos)].z &&
			pos.y < vertexData[calculate_index(tmp_prev_pos)].z + TILE_SIZE)
		{
			//printf("Still in the same tile!\n");
			stillsame++;
		}

		// TODO: maybe check if new pos is in the neighboring tile

		// stop if droplet left the map
		if (pos.x < vertexData[0].x || pos.y < vertexData[0].z || pos.x >= vertexData[0].x + sectorSizeX - 1 || pos.y >= vertexData[0].z + sectorSizeY - 1)
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

	// for (int i = 0; i < pointsPerSectorLine; i++)
	// {
	// 	for (int j = 0; j < pointsPerSectorLine; j++)
	// 	{
	// 		fprintf(heightmap_file, "%.2f ", vertexData[(j + i * (pointsPerSectorLine))].y);
	// 	}
	// 
	// 	fprintf(heightmap_file, "\n");
	// }
	// 
	// fprintf(heightmap_file, "\n\n\n");
}



vec2 SimplexTerrainChunk::calculate_gradient(vec2 position)
{
	// ZoneScoped;

	// calculate index of vertex which is closest top-left point in the mesh
	int index = calculate_index(position);

	float h = vertexData[index].y; // NW
	float h_x = vertexData[index + pointsPerSectorLine].y; // SW
	float h_z = vertexData[index + 1].y; // NE
	float h_xz = vertexData[index + pointsPerSectorLine + 1].y; // SE

	// calculate u and v, which are offsets
	float u = position.x - vertexData[index].x; // x
	float v = position.y - vertexData[index].z; // y

	// calculate gradient
	float x_factor = (h_x - h) * (1 - v) + (h_xz - h_z) * v;
	float z_factor = (h_z - h) * (1 - u) + (h_xz - h_x) * u;

	// generate vector and normalize it, to be 'one tile' long
	vec2 g(x_factor, z_factor);
	// return normalize(g) * ((float)terrain_size / pointsPerSectorLine);
	return g;
}


float SimplexTerrainChunk::calculate_height(vec2 position)
{
	// ZoneScoped;

	int index = calculate_index(position);

	float x1 = vertexData[index].x;
	float x2 = vertexData[index + pointsPerSectorLine].x;

	float z1 = vertexData[index].z;
	float z2 = vertexData[index + 1].z;

	// calculate two linear interpolations in x-direction 
	float x_dir_1 = (x2 - position.x) / (x2 - x1) * vertexData[index].y + (position.x - x1) / (x2 - x1) * vertexData[index + pointsPerSectorLine].y;
	float x_dir_2 = (x2 - position.x) / (x2 - x1) * vertexData[index + 1].y + (position.x - x1) / (x2 - x1) * vertexData[index + pointsPerSectorLine + 1].y;

	// calculate bilinear interpolation, using x-direction interpolations calculated above
	return (z2 - position.y) / (z2 - z1) * x_dir_1 + (position.y - z1) / (z2 - z1) * x_dir_2;
}


void SimplexTerrainChunk::sediment_deposit(vec2 position, float amount)
{
	// ZoneScoped;

	int index = calculate_index(position);

	// calculate u and v, which are offsets
	float u = position.x - vertexData[index].x;
	float v = position.y - vertexData[index].z;

	// float tmp_amount = 0;
	// tmp_amount += amount * (1 - u) * (1 - v);
	// if (tmp_amount > amount)
	// {
	// 	printf("Too much\n");
	// }
	// vertexData[index].y += amount * (1 - u) * (1 - v);
	// 
	// tmp_amount += amount * u * (1 - v);
	// if (tmp_amount > amount)
	// {
	// 	printf("Too much\n");
	// }
	// vertexData[index + 1].y += amount * u * (1 - v);
	// 
	// tmp_amount += amount * (1 - u) * v;
	// if (tmp_amount > amount)
	// {
	// 	printf("Too much\n");
	// }
	// vertexData[index + pointsPerSectorLine].y += amount * (1 - u) * v;
	// 
	// tmp_amount += amount * u * v;
	// if (tmp_amount > amount + 0.0001)
	// {
	// 	printf("Too much\n");
	// }
	// vertexData[index + pointsPerSectorLine + 1].y += amount * u * v;

	vertexData[index].y += amount * (1 - u) * (1 - v);
	vertexData[index + 1].y += amount * u * (1 - v);
	vertexData[index + pointsPerSectorLine].y += amount * (1 - u) * v;
	vertexData[index + pointsPerSectorLine + 1].y += amount * u * v;
}


float SimplexTerrainChunk::sediment_erosion(vec2 position, float amount)
{
	// ZoneScoped;

	int index = calculate_index(position);

	// calculate index of top-left vertex of the square, which will be used to calculate erosion
	int row = index / pointsPerSectorLine;
	int col = index % pointsPerSectorLine;

	int top_row = max(0, row - (erosion_droplet_radius - 1));
	int left_col = max(0, col - (erosion_droplet_radius - 1));

	int top_left_index = left_col + top_row * pointsPerSectorLine;

	int x_radius = min(pointsPerSectorLine - 1, col + erosion_droplet_radius) - left_col;
	int z_radius = min(pointsPerSectorLine - 1, row + erosion_droplet_radius) - top_row;

	float weight_sum = 0;

	// calculate weights sum
	for (int i = 0; i < x_radius; i++)
	{
		for (int j = 0; j < z_radius; j++)
		{
			vec2 point_pos(vertexData[top_left_index + i + (j * pointsPerSectorLine)].x, vertexData[top_left_index + i + (j * pointsPerSectorLine)].z);
			weight_sum += max(0.f, (erosion_droplet_radius - distance(point_pos, position)));
		}
	}

	float sediment = 0;

	// perform erosion for each point
	for (int i = 0; i < x_radius; i++)
	{
		for (int j = 0; j < z_radius; j++)
		{
			vec2 point_pos(vertexData[top_left_index + i + (j * pointsPerSectorLine)].x, vertexData[top_left_index + i + (j * pointsPerSectorLine)].z);
			float weight = max(0.f, (erosion_droplet_radius - distance(point_pos, position)));

			float wighted_amount = amount * weight / weight_sum;

			// calculate delta sediment for terrain, not to drop below 0
			float height = vertexData[top_left_index + i + (j * pointsPerSectorLine)].y;
			float d_sediment = height < wighted_amount ? height : wighted_amount;

			// erode point
			vertexData[top_left_index + i + (j * pointsPerSectorLine)].y -= d_sediment;

			// gather collected sediment
			sediment += d_sediment;
		}
	}

	return sediment;
}



int SimplexTerrainChunk::calculate_index(vec2 position)
{
	// ZoneScoped;

	// calculate x and y, which are indexes of closest top-left point in the mesh
	int x = (position.x - vertexData[0].x) / ((float)sectorSizeX / pointsPerSectorLine);
	int z = (position.y - vertexData[0].z) / ((float)sectorSizeY / pointsPerSectorLine);

	return z + x * (pointsPerSectorLine);
}

void SimplexTerrainChunk::initialize()
{
	ZoneScoped;

	for (int i = 0; i < pointsPerSectorLine; i++)
	{
		for (int j = 0; j < pointsPerSectorLine; j++)
		{
			vertexData[(j + i * (pointsPerSectorLine))] = vec3(i * (float)sectorSizeX / pointsPerSectorLine - sectorSizeX / 2, 0, j * (float)sectorSizeY / pointsPerSectorLine - sectorSizeY / 2);
		}
	}

	// Init indices data
	for (int i = 0; i < pointsPerSectorLine - 1; i++)
	{
		for (int j = 0; j < pointsPerSectorLine - 1; j++)
		{
			indicesData.push_back(j + i * pointsPerSectorLine);
			indicesData.push_back(j + (i + 1) * pointsPerSectorLine);
			indicesData.push_back((j + 1) + i * pointsPerSectorLine);

			indicesData.push_back((j + 1) + i * pointsPerSectorLine);
			indicesData.push_back(j + (i + 1) * pointsPerSectorLine);
			indicesData.push_back((j + 1) + (i + 1) * pointsPerSectorLine);
		}
	}

	// Attributes setup and global VAO creation

	// bind global VAO object
	glBindVertexArray(mainVao.id);

	// select indices VBO (to perform VBO indexing)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer.id);
	// bind indices VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(int), &indicesData[0], GL_DYNAMIC_DRAW);

	updateHeightmap();
}

void SimplexTerrainChunk::update()
{
	// Perform erosion on generated terrain
	// perform_erosion();
}

void SimplexTerrainChunk::sendAndRender()
{
	ZoneScoped;

	this->shaderPtr->set_vec3("material.ambient", vec3(0.298f, 0.282f, 0.27f));
	this->shaderPtr->set_vec3("material.diffuse", vec3(0.458f, 0.411f, 0.341f));
	this->shaderPtr->set_vec3("material.specular", vec3(1.0f, 1.0f, 1.0f));
	this->shaderPtr->set_float("material.shininess", 512);

	// Attributes setup and global VAO creation
	// bind global VAO object
	glBindVertexArray(mainVao.id);

	//// 1st vertex shader input attribute - vertex data
	// select vertex VBO
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pointsPerSectorLine * pointsPerSectorLine, vertexData, GL_DYNAMIC_DRAW);
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
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer.id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pointsPerSectorLine * pointsPerSectorLine, normalsData, GL_DYNAMIC_DRAW);
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

	// Draw
	glDrawElements(
		GL_TRIANGLES,			// mode
		indicesData.size(),		// count
		GL_UNSIGNED_INT,		// type
		(void*)0				// element array buffer offset
	);
}

Shader* SimplexTerrainChunk::getShader()
{
	return shaderPtr;
}

SimplexTerrainChunk::~SimplexTerrainChunk()
{
	indicesData.clear();
	delete[] vertexData;
	delete[] normalsData;
}

void SimplexTerrainChunk::updateSimplexNoise(SimplexNoiseWrapper simplexNoiseParams)
{
	this->simplexNoiseParams = simplexNoiseParams;
	updateHeightmap();
}

SimplexNoiseWrapper SimplexTerrainChunk::getSimplexNoise()
{
	return simplexNoiseParams;
}

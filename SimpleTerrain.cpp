#include "SimpleTerrain.h"

SimpleTerrain::SimpleTerrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number, Shader* p_shader, CameraController* p_camera_controller)
	: Terrain(start_pos_x, start_pos_z, terrain_size, vertices_number, p_shader, p_camera_controller)
{
	initialize_OGL_objects();
}


void SimpleTerrain::init_indices_data()
{
	// init indices data
	indices_data.reserve((vertices_number - 1) * (vertices_number - 1));

	for (int i = 0; i < vertices_number - 1; i++)
	{
		for (int j = 0; j < vertices_number - 1; j++)
		{
			indices_data.push_back(j + i * vertices_number);
			indices_data.push_back(j + (i + 1) * vertices_number);
			indices_data.push_back((j + 1) + i * vertices_number);

			indices_data.push_back((j + 1) + i * vertices_number);
			indices_data.push_back(j + (i + 1) * vertices_number);
			indices_data.push_back((j + 1) + (i + 1) * vertices_number);
		}
	}
}


void SimpleTerrain::init_normals_data()
{
	// init normals data
	normals_data = new vec3[vertices_number * vertices_number];

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

			if (j > 0 && i < vertices_number - 1)
			{
				normal += cross((current - vertex_data[((j - 1) + i * (vertices_number))]), (vertex_data[((j - 1) + (i + 1) * (vertices_number))] - current));
			}

			if (i < vertices_number - 1 && j < vertices_number - 1)
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
}


void SimpleTerrain::initialize_OGL_objects()
{
	// Initialize OGL objects

	// Init Vertex array object and set it as current
	glGenVertexArrays(1, &main_VAO_id);

	// Init vertex buffer
	glGenBuffers(1, &vertices_VBO_id);

	// Init indices buffer
	glGenBuffers(1, &indices_VBO_id);

	// Init normals buffer
	glGenBuffers(1, &normals_VBO_id);
}


void SimpleTerrain::prepare_data()
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

	// 2nd vertex shader input attribute - normals
	// select normals VBO
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO_id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices_number * vertices_number, normals_data, GL_STATIC_DRAW);
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

	get_MVP_handles();
}


SimpleTerrain::~SimpleTerrain()
{
	// Delete OGL objects
	glDeleteVertexArrays(1, &main_VAO_id);
	glDeleteBuffers(1, &vertices_VBO_id);
	glDeleteBuffers(1, &indices_VBO_id);
	glDeleteBuffers(1, &normals_VBO_id);

	delete vertex_data;
	delete normals_data;
}
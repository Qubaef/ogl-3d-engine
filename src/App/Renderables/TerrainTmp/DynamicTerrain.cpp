//#include "DynamicTerrain.h"
//#include "Constants.h"
//
//DynamicTerrain::DynamicTerrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number, Shader* p_shader, CameraController* p_camera_controller)
//	: _Terrain(start_pos_x, start_pos_z, terrain_size, vertices_number, p_shader, p_camera_controller)
//{
//	ZoneScoped;
//	
//	prev_camera_position = p_camera_controller->getPosition();
//
//	// Initialize all needed ogl buffers
//	initialize_OGL_objects();
//
//	// Generate all points of mesh
//	generate_terrain();
//
//	// Initialize quadTree which will be used for meshing
//	p_quad_tree_lod = new QuadTreeLOD(vertex_data, vertices_number, lod_level);
//
//	// TODO: temporary (until texturing)
//	// set material of the terrain
//	set_material();
//
//	// Gather MVP handles
//	get_MVP_handles();
//}
//
//
//void DynamicTerrain::initialize_OGL_objects()
//{
//	ZoneScoped;
//	
//	// Initialize OGL objects
//
//	// Init Vertex array object and set it as current
//	glGenVertexArrays(1, &main_VAO_id);
//
//	// Init vertex buffer
//	glGenBuffers(1, &vertices_VBO_id);
//
//	// Init indices buffer
//	glGenBuffers(1, &indices_VBO_id);
//
//	// Init normals buffer
//	glGenBuffers(1, &normals_VBO_id);
//}
//
//
//void DynamicTerrain::generate_terrain()
//{
//	ZoneScoped;
//	
//	// init vertex data
//	vertex_data = new vec3[vertices_number * vertices_number];
//
//	for (int i = 0; i < vertices_number; i++)
//	{
//		for (int j = 0; j < vertices_number; j++)
//		{
//			vertex_data[(j + i * (SECTOR_DENSITY))] = vec3(start_pos_x + i * (float)SECTOR_SIZE / SECTOR_DENSITY, 0, start_pos_z + j * (float)SECTOR_SIZE / SECTOR_DENSITY);
//		}
//	}
//
//	// All vertices will be send to GPU during mesh initialization
//	// We will then select specific vertices, using indices buffer
//	// 
//	// Bind to VAO to perform draw operation
//	glBindVertexArray(main_VAO_id);
//
//	//// 1st vertex shader input attribute - vertex data
//	// select vertex VBO
//	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO_id);
//	// copy data to gpu memory (to VBO)
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices_number * vertices_number, vertex_data, GL_STATIC_DRAW);
//	// redirect buffer to input of the shader
//	glVertexAttribPointer(
//		0,						// location in shader
//		3,						// size
//		GL_FLOAT,				// type
//		GL_FALSE,				// normalized
//		0,						// stride (space between following fragments)
//		(void*)0				// array buffer offset
//	);
//	// enable attribute '0'
//	glEnableVertexAttribArray(0);
//
//	// 2nd vertex shader input attribute - normals
//	// select normals VBO
//	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO_id);
//	// copy data to gpu memory (to VBO)
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices_number * vertices_number, normals_data, GL_STATIC_DRAW);
//	// redirect buffer to input of the shader
//	glVertexAttribPointer(
//		1,                      // location in shader
//		3,                      // size
//		GL_FLOAT,               // type
//		GL_FALSE,               // normalized
//		0,                      // stride (space between following fragments)
//		(void*)0                // array buffer offset
//	);
//	// enable attribute '1'
//	glEnableVertexAttribArray(1);
//
//	// TODO: temporary
//	// init indices data
//	init_indices_data();
//
//	// TODO: temporary
//	// init normals data
//	init_normals_data();
//}
//
//
//void DynamicTerrain::set_material()
//{
//	ZoneScoped;
//	
//	// Set material
//	p_shader->use();
//	p_shader->set_vec3("material.ambient", vec3(0.35f, 0.3f, 0.254f));
//	p_shader->set_vec3("material.diffuse", vec3(0.45f, 0.4f, 0.354f));
//	p_shader->set_vec3("material.colorSpecular", vec3(0.55f, 0.5f, 0.454f));
//	p_shader->set_float("material.shininess", 1);
//}
//
//
//void DynamicTerrain::init_indices_data()
//{
//	ZoneScoped;
//	
//	// init indices data
//	indices_data.reserve((vertices_number - 1) * (vertices_number - 1));
//
//	for (int i = 0; i < vertices_number - 1; i++)
//	{
//		for (int j = 0; j < vertices_number - 1; j++)
//		{
//			indices_data.push_back(j + i * vertices_number);
//			indices_data.push_back(j + (i + 1) * vertices_number);
//			indices_data.push_back((j + 1) + i * vertices_number);
//
//			indices_data.push_back((j + 1) + i * vertices_number);
//			indices_data.push_back(j + (i + 1) * vertices_number);
//			indices_data.push_back((j + 1) + (i + 1) * vertices_number);
//		}
//	}
//}
//
//
//void DynamicTerrain::init_normals_data()
//{
//	ZoneScoped;
//	
//	// init normals data
//	normals_data = new vec3[vertices_number * vertices_number];
//
//	for (int i = 0; i < vertices_number; i++)
//	{
//		for (int j = 0; j < vertices_number; j++)
//		{
//			vec3 current = vertex_data[(j + i * (vertices_number))];
//
//			// init normal vector
//			vec3 normal = vec3(0, 0, 0);
//
//			if (i > 0 && j < vertices_number - 1)
//			{
//				normal += cross((vertex_data[((j + 1) + (i - 1) * (vertices_number))] - current), (current - vertex_data[(j + (i - 1) * (vertices_number))]));
//			}
//
//			if (j > 0 && i < vertices_number - 1)
//			{
//				normal += cross((current - vertex_data[((j - 1) + i * (vertices_number))]), (vertex_data[((j - 1) + (i + 1) * (vertices_number))] - current));
//			}
//
//			if (i < vertices_number - 1 && j < vertices_number - 1)
//			{
//				normal += cross((current - vertex_data[(j + (i + 1) * (vertices_number))]), (vertex_data[((j + 1) + i * (vertices_number))] - current));
//			}
//
//			// normalize vector's length
//			if (length(normal) == 0)
//			{
//				normal.y = 1;
//			}
//
//			normals_data[(j + i * (vertices_number))] = normalize(normal);
//		}
//	}
//}
//
//
//void DynamicTerrain::prepare_data()
//{
//	ZoneScoped;
//	
//	// Get camera position
//	vec3 camera_position = p_camera_controller->getPosition();
//
//	//// Generate lod mesh if it is require
//	if (prev_camera_position != camera_position)
//	{
//		indices_data.clear();
//		p_quad_tree_lod->create_lod_mesh(indices_data, camera_position);
//		prev_camera_position = camera_position;
//	}
//
//	//// Send data to GPU
//
//	// Attributes setup and global VAO creation
//	// bind global VAO object
//	glBindVertexArray(main_VAO_id);
//	p_shader->use();
//
//	// select indices VBO (to perform VBO indexing)
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_VBO_id);
//	// bind indices VBO
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_data.size() * sizeof(unsigned int), &indices_data[0], GL_DYNAMIC_DRAW);
//}
//
//
//void DynamicTerrain::render_terrain()
//{
//	ZoneScoped;
//	
//	// call universal render_terrain() method
//	_Terrain::render_terrain();
//
//	// prepare data and send it to GPU before drawing
//	prepare_data();
//
//	// Bind to VAO to perform draw operation
//	glBindVertexArray(main_VAO_id);
//
//	// Send view position for colorSpecular component
//	p_shader->set_vec3("view_pos", p_camera_controller->getPosition());
//
//	glDrawElements(
//		GL_TRIANGLES,			// mode
//		indices_data.size(),	// count
//		GL_UNSIGNED_INT,		// type
//		(void*)0				// element array buffer offset
//	);
//}
//
//
//DynamicTerrain::~DynamicTerrain()
//{
//	ZoneScoped;
//	
//	// Delete OGL objects
//	glDeleteVertexArrays(1, &main_VAO_id);
//	glDeleteBuffers(1, &vertices_VBO_id);
//	glDeleteBuffers(1, &indices_VBO_id);
//	glDeleteBuffers(1, &normals_VBO_id);
//
//	delete vertex_data;
//	delete normals_data;
//}
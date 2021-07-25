//#pragma once
//
//#include <vector>
//#include "_Terrain.h"
//
//
//class SimpleTerrain : public _Terrain
//{
//protected:
//	// Ids of OGL objects
//	GLuint vertices_VBO_id;
//	unsigned indices_VBO_id;
//	GLuint normals_VBO_id;
//
//	// Array storing points data
//	vec3* vertex_data;
//
//	// Vector storing indices data
//	std::vector<unsigned> indices_data;
//
//	// Array storing normals data
//	vec3* normals_data;
//
//	SimpleTerrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number, Shader* p_shader, CameraController* p_camera_controller);
//
//	// Delivers default set of indices for terrain size
//	// called only once; always correct for square mesh
//	void init_indices_data();
//
//	// Delivers default algorithm for calculation of normals
//	// (not the most efficient one; for special cases it is better to implement own solution)
//	void init_normals_data();
//
//	void initialize_OGL_objects() override;
//	void prepare_data() override;
//	
//	virtual void set_material() = 0;
//
//public:
//	~SimpleTerrain() override;
//};

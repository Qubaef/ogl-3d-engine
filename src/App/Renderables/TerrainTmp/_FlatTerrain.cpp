﻿//#include "_FlatTerrain.h"
//#include "Constants.h"
//
//_FlatTerrain::_FlatTerrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number, Shader* p_shader, CameraController* p_camera_controller)
//	: SimpleTerrain(start_pos_x, start_pos_z, terrain_size, vertices_number, p_shader, p_camera_controller)
//{
//	ZoneScoped;
//	
//	generate_terrain();
//	set_material();
//}
//
//
//void _FlatTerrain::generate_terrain()
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
//	// init indices data
//	init_indices_data();
//
//	// init normals data
//	init_normals_data();
//
//	// call prepare_data(), as _FlatTerrain is static
//	prepare_data();
//}
//
//
//void _FlatTerrain::set_material()
//{
//	ZoneScoped;
//	
//	// Set material
//	p_shader->use();
//	p_shader->set_vec3("material.ambient", vec3(0.f, 0.41f, 0.58f));
//	p_shader->set_vec3("material.diffuse", vec3(0.f, 0.61f, 0.78f));
//	p_shader->set_vec3("material.colorSpecular", vec3(0.6f, 0.6f, 0.6f));
//	p_shader->set_float("material.shininess", 128);
//}
//
//
//void _FlatTerrain::render_terrain()
//{
//	ZoneScoped;
//	
//	// call universal render_terrain() method
//	_Terrain::render_terrain();
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
#include "Terrain.h"

Terrain::Terrain(int start_pos_x, int start_pos_z, int terrain_size, int vertices_number, Shader* p_shader, CameraController* p_camera_controller)
	:
	start_pos_x(start_pos_x),
	start_pos_z(start_pos_z),
	terrain_size(terrain_size),
	vertices_number(vertices_number),
	p_shader(p_shader),
	p_camera_controller(p_camera_controller)

{
}

void Terrain::get_MVP_handles()
{
	MatrixID = glGetUniformLocation(p_shader->get_ID(), "MVP");
	ViewMatrixID = glGetUniformLocation(p_shader->get_ID(), "V");
	ModelMatrixID = glGetUniformLocation(p_shader->get_ID(), "M");
}


void Terrain::render_terrain()
{
	// Get pointers to matrices
	mat4* mvp = p_camera_controller->getMVPMatrix();
	mat4* V = p_camera_controller->getViewMatrix();
	mat4* M = p_camera_controller->getModelMatrix();

	// Use terrain's shader and send mvp matrices to uniforms
	p_shader->use();

	// Send transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(*mvp)[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &(*V)[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(*M)[0][0]);

	// Send M_inverted for optimization purposes (it is better to calculate it on cpu)
	p_shader->set_mat3("M_inverted", mat3(transpose(inverse(*M))));
}


Terrain::~Terrain()
{
}

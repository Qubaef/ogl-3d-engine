#pragma once
#include "Includes.h"

class Shader
{
private:
	//// Variables

	// shader program ID
	unsigned ID;

	//// Methods
public:
	// initialize shaders from given filepath
	Shader(const char * vertex_file_path, const char * fragment_file_path);

	// ID getter
	unsigned get_ID();

	// activate the shader
	void use();

	// functions to declare uniforms
	// utility uniform functions
	void set_bool(const std::string &name, bool value) const;
	void set_int(const std::string &name, int value) const;
	void set_float(const std::string &name, float value) const;
	void set_vec3(const std::string &name, vec3 value) const;
	void set_vec3(const std::string &name, float x, float y, float z) const;
	void set_vec4(const std::string &name, const glm::vec4 &value) const;
	void set_vec4(const std::string &name, float x, float y, float z, float w) const;
	void set_mat3(const std::string &name, const glm::mat3 &mat) const;
	void set_mat4(const std::string &name, const glm::mat4 &mat) const;
};
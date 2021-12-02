#pragma once

#include <iostream>

#include "../Include/Common.h"

using namespace glm;

class Shader
{
private:
	//// Variables

	// shader program Id
	unsigned Id;

	//// Methods

	// Shader compilation
	void compile(GLuint shaderId, const char* filepath);
public:
	// Graphical pipeline shaders
	Shader(const char* name, const char* vertexFilepath, const char* fragmentFilepath,
		const char* tessControlFilepath = nullptr,
		const char* tessEvaluationFilepath = nullptr,
		const char* geometryFilepath = nullptr);

	// Compute shader
	Shader(const char* name, const char* computeFilepath);

	const std::string name;

	// Id getter
	unsigned get_ID();

	// activate the shader
	void use();

	// functions to declare uniforms
	// utility uniform functions
	void set_bool(const std::string& name, bool value) const;
	void set_int(const std::string& name, int value) const;
	void set_float(const std::string& name, float value) const;
	void set_vec2(const std::string& name, vec2& value) const;
	void set_vec2(const std::string& name, float x, float y) const;
	void set_vec3(const std::string& name, vec3 value) const;
	void set_vec3(const std::string& name, float x, float y, float z) const;
	void set_vec4(const std::string& name, const glm::vec4& value) const;
	void set_vec4(const std::string& name, float x, float y, float z, float w) const;
	void set_mat3(const std::string& name, const glm::mat3& mat) const;
	void set_mat4(const std::string& name, const glm::mat4& mat) const;
};
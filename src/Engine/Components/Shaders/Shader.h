#pragma once

#include <iostream>

#include "Engine/Include/Common.h"

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
	unsigned getId();

	// Activate the shader
	void use();

	// Functions to set uniforms values
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
};
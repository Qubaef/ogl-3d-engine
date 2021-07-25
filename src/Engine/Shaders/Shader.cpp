#include <fstream>
#include <sstream>
#include <vector>
#include "Shader.h"


void Shader::compile(GLuint shaderId, const char* filepath)
{
	ZoneScoped;

	LOG.INFO("Compiling shader: %s\n", filepath);

	std::string shaderCode;
	std::ifstream shaderStream(filepath, std::ios::in);
	if (shaderStream.is_open()) {
		std::stringstream sstr;
		sstr << shaderStream.rdbuf();
		shaderCode = sstr.str();
		shaderStream.close();
	}
	else {
		LOG.ERROR("Impossible to open: %s.\n", filepath);
		return;
	}

	char const* VertexSourcePointer = shaderCode.c_str();
	glShaderSource(shaderId, 1, &VertexSourcePointer, NULL);
	glCompileShader(shaderId);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shaderId, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		LOG.ERROR("Failed to compile shader %s. Error msg: %s \n", filepath, &VertexShaderErrorMessage[0]);
	}

	LOG.INFO("Successfully compiled shader:  %s compiled.\n", filepath);
}

Shader::Shader(const char* name, const char* vertexFilepath, const char* fragmentFilepath,
	const char* tessControlFilepath, const char* tessEvaluationFilepath, const char* geometryFilepath)
	: name(name)
{
	ZoneScoped;

	// Create the shaders
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint tessControlShaderId = 0;
	GLuint tessEvaluationShaderId = 0;
	GLuint geometryShaderId = 0;

	// Compile shaders
	compile(vertexShaderId, vertexFilepath);
	compile(fragmentShaderId, fragmentFilepath);

	// Link the program
	LOG.INFO("Linking program.\n");
	Id = glCreateProgram();
	glAttachShader(Id, vertexShaderId);
	glAttachShader(Id, fragmentShaderId);

	// Compile and attach optional shaders
	if (tessControlFilepath && tessEvaluationFilepath && geometryFilepath)
	{
		tessControlShaderId = glCreateShader(GL_TESS_CONTROL_SHADER);;
		tessEvaluationShaderId = glCreateShader(GL_TESS_EVALUATION_SHADER);
		geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER);
		compile(tessControlShaderId, tessControlFilepath);
		compile(tessEvaluationShaderId, tessEvaluationFilepath);
		compile(geometryShaderId, geometryFilepath);
		glAttachShader(Id, tessControlShaderId);
		glAttachShader(Id, tessEvaluationShaderId);
		glAttachShader(Id, geometryShaderId);
	}

	glLinkProgram(Id);

	// Check the program
	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetProgramiv(Id, GL_LINK_STATUS, &Result);
	glGetProgramiv(Id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(Id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		LOG.ERROR("Failed to link shaders. Error msg: %s \n", &ProgramErrorMessage[0]);
	}

	LOG.INFO("Shaders successfully compiled and linked.\n");

	// Delete shaders (cause they are already compiled)
	glDetachShader(Id, vertexShaderId);
	glDetachShader(Id, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	// Delete optional shaders
	if (tessControlFilepath && tessEvaluationFilepath && geometryFilepath)
	{
		glDetachShader(Id, tessControlShaderId);
		glDetachShader(Id, tessEvaluationShaderId);
		glDetachShader(Id, geometryShaderId);
		glDeleteShader(tessControlShaderId);
		glDeleteShader(tessEvaluationShaderId);
		glDeleteShader(geometryShaderId);
	}
}

Shader::Shader(const char* name, const char* computeFilepath)
	: name(name)
{
	ZoneScoped;

	// Create the shaders
	GLuint computeShaderId = glCreateShader(GL_COMPUTE_SHADER);

	// Compile shaders
	compile(computeShaderId, computeFilepath);

	// Link the program
	LOG.INFO("Linking program.\n");
	Id = glCreateProgram();
	glAttachShader(Id, computeShaderId);
	glLinkProgram(Id);

	// Check the program
	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetProgramiv(Id, GL_LINK_STATUS, &Result);
	glGetProgramiv(Id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(Id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		LOG.ERROR("Failed to link shaders. Error msg: %s \n", &ProgramErrorMessage[0]);
	}

	LOG.INFO("Compute shader successfully compiled and linked.\n");

	// Delete shaders (cause they are already compiled)
	glDetachShader(Id, computeShaderId);
	glDeleteShader(computeShaderId);
}


unsigned Shader::get_ID()
{
	ZoneScoped;

	return Id;
}


void Shader::use()
{
	ZoneScoped;

	// Use created program
	glUseProgram(Id);
}


// bool uniform method
void Shader::set_bool(const std::string& name, bool value) const
{
	ZoneScoped;

	glUniform1i(glGetUniformLocation(Id, name.c_str()), (int)value);
}

// int uniform method
void Shader::set_int(const std::string& name, int value) const
{
	ZoneScoped;

	glUniform1i(glGetUniformLocation(Id, name.c_str()), value);
}

// float uniform method
void Shader::set_float(const std::string& name, float value) const
{
	ZoneScoped;

	glUniform1f(glGetUniformLocation(Id, name.c_str()), value);
}

// Vec3 uniform methods
void Shader::set_vec3(const std::string& name, vec3 value) const
{
	ZoneScoped;

	glUniform3f(glGetUniformLocation(Id, name.c_str()), value.x, value.y, value.z);
}

void Shader::set_vec3(const std::string& name, float x, float y, float z) const
{
	ZoneScoped;

	glUniform3f(glGetUniformLocation(Id, name.c_str()), x, y, z);
}

// Vec4 uniform methods
void Shader::set_vec4(const std::string& name, const glm::vec4& value) const
{
	ZoneScoped;

	glUniform4fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
}

void Shader::set_vec4(const std::string& name, float x, float y, float z, float w) const
{
	ZoneScoped;

	glUniform4f(glGetUniformLocation(Id, name.c_str()), x, y, z, w);
}

// Mat3 uniform method
void Shader::set_mat3(const std::string& name, const glm::mat3& mat) const
{
	ZoneScoped;

	glUniformMatrix3fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// Mat4 uniform method
void Shader::set_mat4(const std::string& name, const glm::mat4& mat) const
{
	ZoneScoped;

	glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
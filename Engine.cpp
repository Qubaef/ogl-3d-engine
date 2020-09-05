#include "Engine.hpp"

void Engine::set_draw_mode(int draw_mode)
{
	this->draw_mode = draw_mode;
}


bool Engine::initialize_GLFW()
{
	// Initialise GLFW

	glewExperimental = true;												// Needed for core profile

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);									// 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);						// We want OpenGL 4
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);					// To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// We don't want the old OpenGL

	return true;
}


bool Engine::initialize_GLEW()
{
	// Initialise GLEW

	if (p_window == NULL)
	{
		fprintf(stderr, "Window was not properly initialized.\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(p_window);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return false;
	}

	return true;
}


bool Engine::initialize_window()
{
	// Open a window and create its OpenGL context

	p_window = glfwCreateWindow(SCREEN_W, SCREEN_H, SCREEN_NAME, NULL, NULL);

	if (p_window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return false;
	}

	return true;
}


void Engine::initialize_OGL_objects()
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


void Engine::initialize_camera_controller()
{
	// Initialize instance of CameraController

	p_controller = new CameraController(p_window);
}


void Engine::set_OGL_parameters()
{
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(p_window, GLFW_STICKY_KEYS, GL_TRUE);

	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);


	// Disable Vsync (requires time synchronization)
	// glfwSwapInterval(0);

	// Cull triangles which normal is not towards the camera (should be turned on for optimization)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	// Set default background color (will be replaced by skybox probably)
	glClearColor(0.2f, 0.8f, 1.0f, 0.0f);
}


void Engine::process_input()
{
	if (glfwGetKey(p_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(p_window, 1);
		return;
	}

	// Detect 'p' click action

	static bool p_clicked = false;

	if (glfwGetKey(p_window, GLFW_KEY_P) == GLFW_PRESS && p_clicked == false)
	{
		// on 'P' key being pressed
		p_clicked = true;

		if (draw_mode == 0)
		{
			draw_mode = 1;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (draw_mode == 1)
		{
			draw_mode = 0;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	else if (glfwGetKey(p_window, GLFW_KEY_P) != GLFW_PRESS && p_clicked == true)
	{
		p_clicked = false;
	}
}


void Engine::track_time_per_frame()
{
	if (processed_frames > -1) {
		// Regular call of the function
		current_time = glfwGetTime();
		processed_frames++;

		// If last printf() was more than 1 sec ago, print status and reset timer
		if (current_time - last_time >= 1.0) {
			printf("%f ms/frame : %d fps \n", 1000.0 / double(processed_frames), processed_frames);
			processed_frames = 0;
			last_time = current_time;
		}
	}
	else
	{
		// First call of the function
		last_time = glfwGetTime();
		processed_frames = 0;
	}
}


bool Engine::check_errors(const char* location)
{
	GLenum errorCode;
	bool result = false;

	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		printf_s("%s error in %s\n", error.c_str(), location);
		result = true;
	}

	return result;
}



Engine::Engine()
{
	// Initialize engine components

	if (!initialize_GLFW())
	{
		throw InitializationException(EXCEPTION_MSG_INIT, "initialize_GLFW");
	}

	if (!initialize_window())
	{
		throw InitializationException(EXCEPTION_MSG_INIT, "initialize_window");
	}

	if (!initialize_GLEW())
	{
		throw InitializationException(EXCEPTION_MSG_INIT, "initialize_GLEW");
	}

	p_lighting_shader = new Shader(LIGHT_SHADER_PATH_VERTEX, LIGHT_SHADER_PATH_FRAGMENT);
	p_light_manager = new LightManager(p_lighting_shader);

	initialize_camera_controller();
	set_OGL_parameters();
	initialize_OGL_objects();

	// Initialize fields

}


int Engine::run()
{
	// Set lighting shader
	p_lighting_shader->use();

	p_lighting_shader->set_vec3("material.ambient", vec3(0.24f, 0.19f, 0.07f));
	p_lighting_shader->set_vec3("material.diffuse", vec3(0.75f, 0.6f, 0.22f));
	p_lighting_shader->set_vec3("material.specular", vec3(0.62f, 0.55f, 0.36f));
	p_lighting_shader->set_float("material.shininess", 1);

	// Get a handles for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(p_lighting_shader->get_ID(), "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(p_lighting_shader->get_ID(), "V");
	GLuint ModelMatrixID = glGetUniformLocation(p_lighting_shader->get_ID(), "M");

	// Get pointers to matrices
	mat4* mvp = p_controller->getMVPMatrix();
	mat4* V = p_controller->getViewMatrix();
	mat4* M = p_controller->getModelMatrix();

	// init vertex data
	vec3 vertexData[SECTOR_DENSITY * SECTOR_DENSITY];

	for (int i = 0; i < SECTOR_DENSITY; i++)
	{
		for (int j = 0; j < SECTOR_DENSITY; j++)
		{
			vertexData[(j + i * (SECTOR_DENSITY))] = vec3(i * (float)SECTOR_SIZE / SECTOR_DENSITY, (float)std::rand() / RAND_MAX, j * (float)SECTOR_SIZE / SECTOR_DENSITY);
			// vertexData[(j + i * (SECTOR_DENSITY))] = vec3(i * (float)SECTOR_SIZE / SECTOR_DENSITY, (-(i - 1)*(i - 1) - (j - 1) *(j - 1)) / 1000.0, j * (float)SECTOR_SIZE / SECTOR_DENSITY);
			// vertexData[(j + i * (SECTOR_DENSITY))] = vec3(i * (float)SECTOR_SIZE / SECTOR_DENSITY, 0, j * (float)SECTOR_SIZE / SECTOR_DENSITY);
		}
	}


	// init indices data
	std::vector<unsigned int> indicesData;

	for (int i = 0; i < SECTOR_DENSITY - 1; i++)
	{
		for (int j = 0; j < SECTOR_DENSITY - 1; j++)
		{
			indicesData.push_back(j + i * SECTOR_DENSITY);
			indicesData.push_back(j + (i + 1) * SECTOR_DENSITY);
			indicesData.push_back((j + 1) + i * SECTOR_DENSITY);

			indicesData.push_back((j + 1) + i * SECTOR_DENSITY);
			indicesData.push_back(j + (i + 1) * SECTOR_DENSITY);
			indicesData.push_back((j + 1) + (i + 1) * SECTOR_DENSITY);
		}
	}

	// init normals data
	vec3 normalsData[SECTOR_DENSITY * SECTOR_DENSITY];

	for (int i = 0; i < SECTOR_DENSITY; i++)
	{
		for (int j = 0; j < SECTOR_DENSITY; j++)
		{
			vec3 current = vertexData[(j + i * (SECTOR_DENSITY))];

			// init normal vector
			normalsData[(j + i * (SECTOR_DENSITY))] = vec3(0, 0, 0);


			if (i > 0 && j < SECTOR_DENSITY - 1)
			{
				normalsData[(j + i * (SECTOR_DENSITY))] += cross((vertexData[((j + 1) + (i - 1) * (SECTOR_DENSITY))] - current), (current - vertexData[(j + (i - 1) * (SECTOR_DENSITY))]));
			}

			if (j > 0 && i < SECTOR_DENSITY - 1)
			{
				normalsData[(j + i * (SECTOR_DENSITY))] += cross((current - vertexData[((j - 1) + i * (SECTOR_DENSITY))]), (vertexData[((j - 1) + (i + 1) * (SECTOR_DENSITY))] - current));
			}

			if (i < SECTOR_DENSITY - 1 && j < SECTOR_DENSITY - 1)
			{
				normalsData[(j + i * (SECTOR_DENSITY))] += cross((current - vertexData[(j + (i + 1) * (SECTOR_DENSITY))]), (vertexData[((j + 1) + i * (SECTOR_DENSITY))] - current));
			}

			// normalize vector's length
			if (length(normalsData[(j + i * (SECTOR_DENSITY))]) == 0)
			{
				normalsData[(j + i * (SECTOR_DENSITY))].y = 1;
			}

			normalsData[(j + i * (SECTOR_DENSITY))] = normalize(normalsData[(j + i * (SECTOR_DENSITY))]);
		}
	}

	// Attributes setup and global VAO creation

	// bind global VAO object
	glBindVertexArray(vertices_VBO_id);

	// select indices VBO (to perform VBO indexing)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_VBO_id);
	// bind indices VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(unsigned int), &indicesData[0], GL_DYNAMIC_DRAW);

	//// 1st vertex shader input attribute - normals
	// select vertex VBO
	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO_id);
	// copy data to gpu memory (to VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_DYNAMIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalsData), normalsData, GL_DYNAMIC_DRAW);
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

	// Render loop
	while (!glfwWindowShouldClose(p_window))
	{
		// Track time per frame value and print it's status
		track_time_per_frame();

		// Clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Send transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(*mvp)[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &(*V)[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(*M)[0][0]);

		// Send M_inverted for optimization purposes (it is better to calculate it on cpu)
		p_lighting_shader->set_mat3("M_inverted", mat3(transpose(inverse(*M))));
		// Send view position for specular component (TODO: check if can be done better)
		p_lighting_shader->set_vec3("view_pos", p_controller->getPosition());

		// Update lights in the scene
		p_light_manager->update();

		// Bind to vertices to perform draw operation
		glBindVertexArray(vertices_VBO_id);

		glDrawElements(
			GL_TRIANGLES,			// mode
			indicesData.size(),		// count
			GL_UNSIGNED_INT,		// type
			(void*)0				// element array buffer offset
		);

		// Swap buffers after the draw (idk why, apparently it is required)
		glfwSwapBuffers(p_window);

		// Process pending events
		glfwPollEvents();

		// Process non-camera related input
		process_input();

		// Update matrices according to user's input
		p_controller->updateCamera();

		// check for errors
		assert(check_errors("render loop") == false);
	}

	// Cleanup VBO and shader program
	glDeleteVertexArrays(1, &main_VAO_id);
	glDeleteBuffers(1, &vertices_VBO_id);
	glDeleteBuffers(1, &indices_VBO_id);
	glDeleteBuffers(1, &normals_VBO_id);
	glDeleteProgram(p_lighting_shader->get_ID());

	// Cleanup dynamically allocated objects
	delete p_lighting_shader;
	delete p_light_manager;
	delete p_controller;

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

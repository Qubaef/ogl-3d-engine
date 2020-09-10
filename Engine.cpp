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


void Engine::initialize_user_control()
{
	// Initialize InputManager
	p_input_manager = new InputManager(p_window);

	// Initialize instance of CameraController
	//p_controller = new FirstPersonCameraController(p_window, p_input_manager);
	p_controller = new OverviewCameraController(p_window, p_input_manager);
}


void Engine::initialize_terrain()
{
	// p_terrain = new FlatTerrain(0, 0, SECTOR_SIZE, SECTOR_DENSITY);
	// p_terrain = new RandomTerrain(0, 0, SECTOR_SIZE, SECTOR_DENSITY);
	p_terrain = new WaterTerrain(0, 0, SECTOR_SIZE, SECTOR_DENSITY);
}


void Engine::set_OGL_parameters()
{
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(p_window, GLFW_STICKY_KEYS, GL_TRUE);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);


	// Disable Vsync (requires time synchronization)
	//glfwSwapInterval(0);

	// Cull triangles which normal is not towards the camera (should be turned on for optimization)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	// Set default background color (will be replaced by skybox probably)
	glClearColor(0.2f, 0.8f, 1.0f, 0.0f);
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

	initialize_user_control();
	initialize_terrain();
	set_OGL_parameters();

	// Initialize fields

}


int Engine::run()
{
	// Set lighting shader
	p_lighting_shader->use();

	p_lighting_shader->set_vec3("material.ambient", vec3(0.f, 0.41f, 0.58f));
	p_lighting_shader->set_vec3("material.diffuse", vec3(0.f, 0.61f, 0.78f));
	p_lighting_shader->set_vec3("material.specular", vec3(0.6f, 0.6f, 0.6f));
	p_lighting_shader->set_float("material.shininess", 256);

	// Get a handles for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(p_lighting_shader->get_ID(), "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(p_lighting_shader->get_ID(), "V");
	GLuint ModelMatrixID = glGetUniformLocation(p_lighting_shader->get_ID(), "M");

	// Get pointers to matrices
	mat4* mvp = p_controller->getMVPMatrix();
	mat4* V = p_controller->getViewMatrix();
	mat4* M = p_controller->getModelMatrix();


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

		// render terrain
		p_terrain->render_terrain();

		// Swap buffers after the draw (idk why, apparently it is required)
		glfwSwapBuffers(p_window);

		// Process pending events
		glfwPollEvents();

		// Update matrices according to user's input
		p_controller->updateCamera();

		// Process non-camera related input
		p_input_manager->process_input(draw_mode);

		// check for errors
		assert(check_errors("render loop") == false);
	}

	// Cleanup VBO and shader program
	glDeleteProgram(p_lighting_shader->get_ID());

	// Cleanup dynamically allocated objects
	delete p_lighting_shader;
	delete p_light_manager;
	delete p_controller;
	delete p_terrain;
	delete p_input_manager;

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}
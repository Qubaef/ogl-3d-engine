#pragma once

#include "../../Engine/Include/Common.h"
#include "InputState.h"

class Engine;

using namespace glm;

class InputManager
{
	GLFWwindow* p_window;

	InputState input_state;

	// Set custom user pointer, to make binding possible
	void set_window_user_pointer();

	// Bind scroll
	void bind_scroll_event();
	void bind_mouse_click_event();

public:
	InputManager(Engine* enginePtr);

	// Process user's input which happens every frame (mouse_pos)
	// and ESC key, which closes app and should be checked processed no matter the case
	InputState& process_input();

	// Register event for key of given number
	void register_key_event(int glfwKeyNumber);

	// Mouse scroll event
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	// Mouse button click event
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	// Reset mouse position to given one
	void reset_mouse_pos(int xpos, int ypos);

	// Getters
	InputState& get_input_state();
	KeyState getKeyState(int glfwKeyNumber);
	vec2 get_mouse_pos();
};
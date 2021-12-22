#include "InputManager.h"

#include "../../Engine/Engine.h"
#include "KeyState.h"

void InputManager::set_window_user_pointer()
{
	ZoneScoped;

	glfwSetWindowUserPointer(p_window, this);
}

void InputManager::bind_scroll_event()
{
	ZoneScoped;

	auto func = [](GLFWwindow* w, double xoffset, double yoffset)
	{
		static_cast<InputManager*>(glfwGetWindowUserPointer(w))->scroll_callback(w, xoffset, yoffset);
	};

	glfwSetScrollCallback(p_window, func);
}

void InputManager::bind_mouse_click_event()
{
	ZoneScoped;

	auto func = [](GLFWwindow* w, int button, int action, int mods)
	{
		static_cast<InputManager*>(glfwGetWindowUserPointer(w))->mouse_button_callback(w, button, action, mods);
	};

	glfwSetMouseButtonCallback(p_window, func);
}


InputManager::InputManager(Engine* enginePtr)
{
	ZoneScoped;

	p_window = enginePtr->getGlWindow();

	// Set custom user pointer, to make binding possible
	set_window_user_pointer();

	// Bind scroll mouse event
	bind_scroll_event();
	// Bind mouse click event
	bind_mouse_click_event();
}

InputState& InputManager::process_input()
{
	ZoneScoped;

	// Process escape button click (which closes the program)
	if (glfwGetKey(p_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(p_window, 1);
	}

	// Update mouse current pos
	input_state.mouse_pos_prev = input_state.mouse_pos;
	input_state.mouse_pos = get_mouse_pos();

	// Process keyboard inputs
	for (auto& keyState : input_state.keysStates)
	{
		if (glfwGetKey(p_window, keyState.first) == GLFW_PRESS)
		{
			if (keyState.second == NOT_PRESSED)
			{
				keyState.second = JUST_PRESSED;
			}
			else
			{
				keyState.second = HOLD_PRESSED;
			}
		}
		else if (glfwGetKey(p_window, keyState.first) == GLFW_RELEASE && keyState.second != NOT_PRESSED)
		{
			keyState.second = NOT_PRESSED;
		}
	}

	return input_state;
}

void InputManager::register_key_event(int glfwKeyNumber)
{
	ZoneScoped;

	input_state.registerKey(glfwKeyNumber);
}


void InputManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ZoneScoped;

	input_state.mouse_scroll_moved = true;
	input_state.mouse_scroll_y_offset = yoffset;
	input_state.mouse_scroll_position = get_mouse_pos();
}

void InputManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	ZoneScoped;

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			input_state.l_mouse_pressed = true;
			input_state.l_mouse_click_pos = get_mouse_pos();
		}
		else if (action == GLFW_RELEASE)
		{
			input_state.l_mouse_pressed = false;
			input_state.l_mouse_release_pos = get_mouse_pos();
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			input_state.r_mouse_pressed = true;
			input_state.r_mouse_click_pos = get_mouse_pos();
		}
		else if (action == GLFW_RELEASE)
		{
			input_state.r_mouse_pressed = false;
			input_state.r_mouse_release_pos = get_mouse_pos();
		}
	}
}


void InputManager::reset_mouse_pos(int xpos, int ypos)
{
	ZoneScoped;

	glfwSetCursorPos(p_window, xpos, ypos);
}



InputState& InputManager::get_input_state()
{
	ZoneScoped;

	return input_state;
}

KeyState InputManager::getKeyState(int glfwKeyNumber)
{
	return input_state.ifKeyPressed(glfwKeyNumber);
}

vec2 InputManager::get_mouse_pos()
{
	ZoneScoped;

	double x_pos, y_pos;
	glfwGetCursorPos(p_window, &x_pos, &y_pos);
	return vec2(x_pos, y_pos);
}

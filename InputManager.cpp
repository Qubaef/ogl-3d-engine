#include "InputManager.h"

void InputManager::set_window_user_pointer()
{
	glfwSetWindowUserPointer(p_window, this);
}

void InputManager::bind_scroll_event()
{
	auto func = [](GLFWwindow* w, double xoffset, double yoffset)
	{
		static_cast<InputManager*>(glfwGetWindowUserPointer(w))->scroll_callback(w, xoffset, yoffset);
	};

	glfwSetScrollCallback(p_window, func);
}

void InputManager::bind_mouse_click_event()
{
	auto func = [](GLFWwindow* w, int button, int action, int mods)
	{
		static_cast<InputManager*>(glfwGetWindowUserPointer(w))->mouse_button_callback(w, button, action, mods);
	};

	glfwSetMouseButtonCallback(p_window, func);
}


InputManager::InputManager(GLFWwindow* p_window) :
	p_window(p_window)
{
	l_mouse_pressed = false;
	r_mouse_pressed = false;
	mouse_scroll_moved = false;

	// Set custom user pointer, to make binding possible
	set_window_user_pointer();

	// Bind scroll mouse
	bind_scroll_event();
	// Bind mouse click
	bind_mouse_click_event();
}

void InputManager::process_input(int& draw_mode)
{
	// Process escape button click (which closes the program)
	if (glfwGetKey(p_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(p_window, 1);
		return;
	}

	// Update mouse current pos
	mouse_last_pos = get_mouse_pos(p_window);

	// Detect 'p' click action and change polygon mode accordingly

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

vec2 InputManager::get_mouse_pos(GLFWwindow* p_window)
{
	double x_pos, y_pos;
	glfwGetCursorPos(p_window, &x_pos, &y_pos);
	return vec2(x_pos, y_pos);
}


void InputManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mouse_scroll_moved = true;
	mouse_scroll_y_offset = yoffset;
	mouse_scroll_position = get_mouse_pos(p_window);
}

void InputManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		l_mouse_pressed = true;
		l_mouse_click_pos = get_mouse_pos(p_window);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		l_mouse_pressed = false;
		l_mouse_release_pos = get_mouse_pos(p_window);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		r_mouse_pressed = true;
		r_mouse_click_pos = get_mouse_pos(p_window);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		r_mouse_pressed = false;
		r_mouse_release_pos = get_mouse_pos(p_window);
	}
}

vec2 InputManager::get_mouse_last_pos() const
{
	return mouse_last_pos;
}

bool InputManager::if_l_mouse_pressed() const
{
	return l_mouse_pressed;
}

vec2 InputManager::get_l_mouse_click_pos() const
{
	return l_mouse_click_pos;
}

vec2 InputManager::get_l_mouse_release_pos() const
{
	return l_mouse_release_pos;
}


bool InputManager::if_r_mouse_pressed() const
{
	return r_mouse_pressed;
}

vec2 InputManager::get_r_mouse_click_pos() const
{
	return r_mouse_click_pos;
}

vec2 InputManager::get_r_mouse_release_pos() const
{
	return r_mouse_release_pos;
}


bool InputManager::if_mouse_scroll_moved()
{
	if (mouse_scroll_moved)
	{
		mouse_scroll_moved = false;
		return true;
	}
	else
	{
		return false;
	}
}

vec2 InputManager::get_mouse_scroll_position() const
{
	return mouse_scroll_position;
}

double InputManager::get_mouse_scroll_y_offset() const
{
	return mouse_scroll_y_offset;
}
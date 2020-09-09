#pragma once
#include "Includes.hpp"

class InputManager
{
	GLFWwindow* p_window;

	vec2 mouse_last_pos;

	bool l_mouse_pressed;
	vec2 l_mouse_click_pos;
	vec2 l_mouse_release_pos;

	bool r_mouse_pressed;
	vec2 r_mouse_click_pos;
	vec2 r_mouse_release_pos;

	bool mouse_scroll_moved;
	vec2 mouse_scroll_position;
	double mouse_scroll_y_offset;

	// Set custom user pointer, to make binding possible
	void set_window_user_pointer();

	// Bind scroll
	void bind_scroll_event();
	void bind_mouse_click_event();

public:
	InputManager(GLFWwindow* p_window);

	// Process user's input
	void process_input(int& draw_mode);

	// Get mouse packed to vec2
	static vec2 get_mouse_pos(GLFWwindow* p_window);

	// Mouse scroll event
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	// Mouse button click event
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	// Getters
	vec2 get_mouse_last_pos() const;

	bool if_l_mouse_pressed() const;
	vec2 get_l_mouse_click_pos() const;
	vec2 get_l_mouse_release_pos() const;

	bool if_r_mouse_pressed() const;
	vec2 get_r_mouse_click_pos() const;
	vec2 get_r_mouse_release_pos() const;

	bool if_mouse_scroll_moved();
	vec2 get_mouse_scroll_position() const;
	double get_mouse_scroll_y_offset() const;
};
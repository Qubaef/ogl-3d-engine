﻿#pragma once
#include <map>

struct InputState
{
	// Inputs which are always checked
	vec2 mouse_pos;
	vec2 mouse_pos_prev;

	bool l_mouse_pressed;
	vec2 l_mouse_click_pos;
	vec2 l_mouse_release_pos;

	bool r_mouse_pressed;
	vec2 r_mouse_click_pos;
	vec2 r_mouse_release_pos;

	bool mouse_scroll_moved;
	vec2 mouse_scroll_position;
	double mouse_scroll_y_offset;

	enum KeyState
	{
		NOT_PRESSED,
		JUST_PRESSED,
		HOLD_PRESSED
	};

	// Inputs which are specified by user to check them every frame
	// You can specify them by setting key registration in inputManager()
	std::map<int, KeyState> keysStates;

	InputState()
	{
		mouse_pos = vec2(0, 0);

		l_mouse_pressed = false;
		r_mouse_pressed = false;
		mouse_scroll_moved = false;
	}

	// Set registration of key of given number
	void register_key(int glfw_key_number)
	{
		keysStates[glfw_key_number] = NOT_PRESSED;
	}

	// Check if key of given number is pressed
	KeyState if_key_pressed(int glfw_key_number)
	{
		auto it = keysStates.find(glfw_key_number);
		if (it != keysStates.end())
		{
			return it->second;
		}
		else
		{
			return NOT_PRESSED;
		}
	}

	bool if_mouse_scroll_moved()
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
};

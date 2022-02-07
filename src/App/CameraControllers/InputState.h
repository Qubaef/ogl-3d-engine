#pragma once
#include <map>

#include "KeyState.h"
#include "Engine/Include/Common.h"

struct InputState
{
	// Inputs which are always checked
	glm::vec2 mouse_pos;
	glm::vec2 mouse_pos_prev;

	bool l_mouse_pressed;
	glm::vec2 l_mouse_click_pos;
	glm::vec2 l_mouse_release_pos;

	bool r_mouse_pressed;
	glm::vec2 r_mouse_click_pos;
	glm::vec2 r_mouse_release_pos;

	bool mouse_scroll_moved;
	glm::vec2 mouse_scroll_position;
	double mouse_scroll_y_offset;

	// Inputs which are specified by user to check them every frame
	// You can specify them by setting key registration in inputManager()
	std::map<int, KeyState> keysStates;

	InputState()
		: mouse_pos(glm::vec2(0, 0)),
		mouse_pos_prev(glm::vec2(0, 0)),
		l_mouse_pressed(false),
		l_mouse_click_pos(glm::vec2(0, 0)),
		l_mouse_release_pos(glm::vec2(0, 0)),
		r_mouse_pressed(false),
		r_mouse_click_pos(glm::vec2(0, 0)),
		r_mouse_release_pos(glm::vec2(0, 0)),
		mouse_scroll_moved(false),
		mouse_scroll_position(glm::vec2(0, 0)),
		mouse_scroll_y_offset(0)
	{
	}

	// Set registration of key of given number
	void registerKey(int glfw_key_number)
	{
		if (keysStates.find(glfw_key_number) == keysStates.end())
		{
			keysStates[glfw_key_number] = KeyState::NOT_PRESSED;
		}
	}

	// Check if key of given number is pressed
	KeyState ifKeyPressed(int glfw_key_number)
	{
		const auto it = keysStates.find(glfw_key_number);

		if (it != keysStates.end())
		{
			return it->second;
		}

		return KeyState::NOT_PRESSED;
	}

	bool ifMouseScrollMoved()
	{
		if (mouse_scroll_moved)
		{
			mouse_scroll_moved = false;
			return true;
		}

		return false;
	}
};

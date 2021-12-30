#pragma once

class IDisplayable
{
public:
	// Display object using ImGui
	// Object should display just it's data in ImGui context
	// Whole context is created before and after the function call
	virtual void display() = 0;
};

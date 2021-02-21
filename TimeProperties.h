#pragma once

struct TimeProperties
{
	// Variables to track time per frame
	double last_time_print;
	int processed_frames;

	// Variables to track time since last frame
	double last_time_frame;
	double delta_time_frame;

	TimeProperties()
	{
		// First call of the function
		last_time_print = glfwGetTime();
		last_time_frame = last_time_print;
		processed_frames = 0;
		delta_time_frame = 0;
	}
};

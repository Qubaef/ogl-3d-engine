#pragma once

struct TimeProperties
{
	// Variables to track time-per-frame console message
	double lastPrintTimestamp;
	int processedFramesNumber;
	float printInterval;

	// Variables to track time since last frame
	double lastFrameTimestamp;
	double lastFrameDelta;
};

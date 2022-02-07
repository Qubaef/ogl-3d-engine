#pragma once

struct PropertiesTime
{
	// Variables to track time-per-frame console message
	mutable double lastPrintTimestamp;
	int processedFramesNumber;
	float printInterval;

	// Variables to track time since last frame
	double lastFrameTimestamp;
	double lastFrameDelta;
};

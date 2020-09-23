#pragma once
#include "QuadPoint.h"

// TODO: add circle to square collision detection to perform better LOD

class Quad
{
	// Original vector of indices, which will be eventually filled
	std::vector<unsigned>& indices;

	// Points which are part of quad
	QuadPoint** corners_points;
	QuadPoint** sides_points;
	QuadPoint mid_point;

	// Fill indices vertex with smallest rectangles in the range of the quad
	void fill_all();

public:
	Quad(std::vector<unsigned>& indices, QuadPoint* corners_points[], QuadPoint* sides_points[], QuadPoint& mid_point);

	// Methods to validate children of the quad
	// These methods are filling indices buffer if required
	// If returned value is 'true', there should be recursive split performed
	// If returned value if 'false', all there shouldn't be split performed

	bool split_top_left();
	bool split_top_right();
	bool split_bot_left();
	bool split_bot_right();
};

#pragma once

#include "../../../Engine/Include/Common.h"

using namespace glm;

class QuadPoint
{
public:
	QuadPoint(vec2 point_coords, int point_index, vec3& camera_position, float lod_level);

	vec2 point_coords;
	int index;
	bool enable;
};

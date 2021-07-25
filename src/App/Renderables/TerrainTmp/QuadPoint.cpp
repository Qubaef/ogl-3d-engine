#include "QuadPoint.h"

QuadPoint::QuadPoint(vec2 point_coords, int point_index, vec3& camera_position, float lod_level) :
	point_coords(point_coords), index(point_index)
{
	ZoneScoped;
	
	// float n = max(abs(point_coords.x - camera_position.x), abs(point_coords.y - camera_position.z));
	float n = sqrt(pow(camera_position.x - point_coords.x, 2) + pow(camera_position.z - point_coords.y, 2));
	enable = lod_level >= n;
}
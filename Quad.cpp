#include "Quad.h"

Quad::Quad(std::vector<unsigned>& indices, std::vector<QuadPoint>& corners_points, std::vector<QuadPoint>& sides_points, QuadPoint& mid_point) :
	indices(indices), corners_points(corners_points), sides_points(sides_points), mid_point(mid_point)
{
	// Check if are corners are enabled
	// If so, there is no point of splitting quad
	// and indices vector should be filled with all smallest indices
	if (corners_points[0].enable && corners_points[1].enable && corners_points[2].enable && corners_points[3].enable)
	{
		fill_all();
	}
}


void Quad::fill_all()
{
	int d = corners_points[1].index - corners_points[0].index;
	int y_offset = (corners_points[2].index - corners_points[0].index) / d;

	// Iterate through all smallest rectangles and push them to vertices buffer
	for (int row = corners_points[0].index; row < corners_points[2].index; row += y_offset)
	{
		for (int i = 0; i < d; i++)
		{
			indices.push_back(row + i);
			indices.push_back(row + i + y_offset);
			indices.push_back(row + i + 1);

			indices.push_back(row + i + 1);
			indices.push_back(row + i + y_offset);
			indices.push_back(row + i + y_offset + 1);
		}
	}
}



bool Quad::split_top_left()
{
	// Check if at least one vertex is enabled
	if (mid_point.enable || corners_points[0].enable || sides_points[0].enable
		|| sides_points[3].enable)
	{
		// If not all vertices are enabled, return 'true' to perform split
		return true;
	}
	else
	{
		// If there are no vertices enabled, indices should be filled with "big rectangle" data
		indices.push_back(corners_points[0].index);
		indices.push_back(sides_points[3].index);
		indices.push_back(sides_points[0].index);

		indices.push_back(sides_points[0].index);
		indices.push_back(sides_points[3].index);
		indices.push_back(mid_point.index);

		return false;
	}
}


bool Quad::split_top_right()
{
	// Check if at least one vertex is enabled
	if (mid_point.enable || corners_points[1].enable || sides_points[0].enable
		|| sides_points[1].enable)
	{
		// If not all vertices are enabled, return 'true' to perform split
		return true;
	}
	else
	{
		// If there are no vertices enabled, indices should be filled with "big rectangle" data
		indices.push_back(sides_points[0].index);
		indices.push_back(mid_point.index);
		indices.push_back(corners_points[1].index);

		indices.push_back(corners_points[1].index);
		indices.push_back(mid_point.index);
		indices.push_back(sides_points[1].index);

		return false;
	}
}


bool Quad::split_bot_left()
{
	// Check if at least one vertex is enabled
	if (mid_point.enable || corners_points[2].enable || sides_points[3].enable
		|| sides_points[2].enable)
	{
		// If not all vertices are enabled, return 'true' to perform split
		return true;
	}
	else
	{
		// If there are no vertices enabled, indices should be filled with "big rectangle" data
		indices.push_back(sides_points[3].index);
		indices.push_back(corners_points[2].index);
		indices.push_back(mid_point.index);

		indices.push_back(mid_point.index);
		indices.push_back(corners_points[2].index);
		indices.push_back(sides_points[2].index);

		return false;
	}
}


bool Quad::split_bot_right()
{
	// Check if at least one vertex is enabled
	if (mid_point.enable || corners_points[3].enable || sides_points[1].enable
		|| sides_points[2].enable)
	{
		// If not all vertices are enabled, return 'true' to perform split
		return true;
	}
	else
	{
		// If there are no vertices enabled, indices should be filled with "big rectangle" data
		indices.push_back(mid_point.index);
		indices.push_back(sides_points[2].index);
		indices.push_back(sides_points[1].index);

		indices.push_back(sides_points[1].index);
		indices.push_back(sides_points[2].index);
		indices.push_back(corners_points[3].index);

		return false;
	}
}

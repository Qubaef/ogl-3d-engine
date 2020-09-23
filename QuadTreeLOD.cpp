#include "QuadTreeLOD.h"

QuadTreeLOD::QuadTreeLOD(vec3* mesh_array, int mesh_size, float lod_level) :
	mesh_array(mesh_array), mesh_size(mesh_size), lod_level(lod_level)
{
}


void QuadTreeLOD::create_lod_mesh(std::vector<unsigned>& indices, vec3 camera_position)
{
	quad_step(indices, camera_position, 0, mesh_size - 1, mesh_size * (mesh_size - 1), mesh_size * mesh_size - 1);
}


void QuadTreeLOD::quad_step(std::vector<unsigned>& indices, vec3& camera_position, int tl, int tr, int bl, int br)
{
	int tm = (tl + tr) / 2;
	int bm = (bl - tl) + tm;

	int ml = int((bl - tl) / (mesh_size * 2)) * mesh_size + tl;
	int mr = (tr - tl) + ml;

	// Check if quad is splittable
	if (tm > tl && ml - tl >= mesh_size)
	{
		// Initialize memory containers

		QuadPoint* corners_points[4];
		QuadPoint* sides_points[4];

		// Calculate mid point
		int curr_i = ml + (tm - tl);
		QuadPoint mid_point(vec2(mesh_array[curr_i].x, mesh_array[curr_i].z), curr_i, camera_position, lod_level);

		// Calculate corners
		// top left
		curr_i = tl;
		corners_points[0] = new QuadPoint(vec2(mesh_array[curr_i].x, mesh_array[curr_i].z), curr_i, camera_position, lod_level);
		// top right
		curr_i = tr;
		corners_points[1] = new QuadPoint(vec2(mesh_array[curr_i].x, mesh_array[curr_i].z), curr_i, camera_position, lod_level);
		// bot left
		curr_i = bl;
		corners_points[2] = new QuadPoint(vec2(mesh_array[curr_i].x, mesh_array[curr_i].z), curr_i, camera_position, lod_level);
		// bot right
		curr_i = br;
		corners_points[3] = new QuadPoint(vec2(mesh_array[curr_i].x, mesh_array[curr_i].z), curr_i, camera_position, lod_level);

		// Calculate sides
		// top
		curr_i = tm;
		sides_points[0] = new QuadPoint(vec2(mesh_array[curr_i].x, mesh_array[curr_i].z), curr_i, camera_position, lod_level);
		// right
		curr_i = mr;
		sides_points[1] = new QuadPoint(vec2(mesh_array[curr_i].x, mesh_array[curr_i].z), curr_i, camera_position, lod_level);
		// bot
		curr_i = bm;
		sides_points[2] = new QuadPoint(vec2(mesh_array[curr_i].x, mesh_array[curr_i].z), curr_i, camera_position, lod_level);
		// left
		curr_i = ml;
		sides_points[3] = new QuadPoint(vec2(mesh_array[curr_i].x, mesh_array[curr_i].z), curr_i, camera_position, lod_level);

		// Init quad
		Quad quad(indices, corners_points, sides_points, mid_point);

		// check top left quad
		if (quad.split_top_left())
		{
			quad_step(indices, camera_position, corners_points[0]->index, sides_points[0]->index, sides_points[3]->index, mid_point.index);
		}

		// check top right quad
		if (quad.split_top_right())
		{
			quad_step(indices, camera_position, sides_points[0]->index, corners_points[1]->index, mid_point.index, sides_points[1]->index);
		}

		// check bottom left quad
		if (quad.split_bot_left())
		{
			quad_step(indices, camera_position, sides_points[3]->index, mid_point.index, corners_points[2]->index, sides_points[2]->index);
		}

		// check bottom right quad
		if (quad.split_bot_right())
		{
			quad_step(indices, camera_position, mid_point.index, sides_points[1]->index, sides_points[2]->index, corners_points[3]->index);
		}

		// delete initialized objects after using them
		for (int i = 0; i < 4; i++)
		{
			delete corners_points[i];
			delete sides_points[i];
		}
	}
	else
	{
		indices.push_back(tl);
		indices.push_back(bl);
		indices.push_back(tr);

		indices.push_back(tr);
		indices.push_back(bl);
		indices.push_back(br);
	}
}
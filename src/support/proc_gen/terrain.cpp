#include <glge/support/proc_gen/terrain.h>

#include "terrain_mesh.h"
#include "diamond_square.h"

#include <cmath>

namespace glge::proc_gen
{
	using namespace util;

	Matrix<float>
		diamond_square_gen(std::uint32_t seed, size_t n, float variation, float top_left,
			float top_right, float bottom_left, float bottom_right)
	{
		const size_t grid_sz = static_cast<size_t>(pow(2, n)) + 1;

		Matrix<float> grid(grid_sz, grid_sz);

		grid.at(0, 0) = top_left;
		grid.at(grid_sz - 1, 0) = top_right;
		grid.at(0, grid_sz - 1) = bottom_left;
		grid.at(grid_sz - 1, grid_sz - 1) = bottom_right;

		diamond_square(seed, grid, variation);

		/*
		vector<vec3> heightmap(grid_sz * grid_sz);

		for (size_t x = 0; x < grid_sz; x++)
		{
			for (size_t z = 0; z < grid_sz; z++)
			{
				heightmap.at(x + grid_sz * z) = vec3(x, grid.at(x, z), z);
			}
		}

		// TODO move this conversion to resources namespace
		//auto indices = heightmap_indices(grid.width(), grid.height());
		//auto normals = heightmap_normals(heightmap, indices);
		//auto uvs = heightmap_uvs(heightmap, grid.width(), grid.height());
		*/

		return std::move(grid);
	}
}

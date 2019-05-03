#include <glge/support/proc_gen/terrain.h>

#include "diamond_square.h"

#include <cmath>

namespace glge::proc_gen
{
	using namespace util;

	Matrix<float> diamond_square_gen(std::uint32_t seed,
									 size_t n,
									 float variation,
									 float top_left,
									 float top_right,
									 float bottom_left,
									 float bottom_right)
	{
		const size_t grid_sz = static_cast<size_t>(std::pow(2, n)) + 1;

		Matrix<float> grid(grid_sz, grid_sz);

		grid.at(0, 0) = top_left;
		grid.at(grid_sz - 1, 0) = top_right;
		grid.at(0, grid_sz - 1) = bottom_left;
		grid.at(grid_sz - 1, grid_sz - 1) = bottom_right;

		diamond_square(seed, grid, variation);

		return grid;
	}
}   // namespace glge::proc_gen

#include "DiamondSquare.h"

namespace eng
{
	namespace procgen
	{
		void diamond_square(std::seed_seq & seed, Matrix<float> & grid, float variation)
		{
			Engine generator(seed);
			size_t radius = static_cast<size_t>(grid.width()) / 2;

			while (radius > 0)
			{
				Distribution dist(-variation, variation);

				auto rng = [&] { return dist(generator); };

				square_step(rng, grid, radius);
				diamond_step(rng, grid, radius);

				variation /= 2.0f;
				radius = radius / 2;
			}
		}

		void diamond_step(const RNG & rng, Matrix<float> & grid, size_t radius)
		{
			auto get_diamond_f = [&grid, radius](grid_pt center) { return get_diamond(grid, radius, center); };
			auto diamond_f = [&](grid_pt && center) { return aggregate(get_diamond_f, rng, std::forward<grid_pt>(center)); };

			bool offset = true;
			for (size_t x = 0; x < grid.width(); x += radius)
			{
				for (size_t y = offset ? radius : 0; y < grid.height(); y += (2 * radius))
				{
					grid.at(x, y) = diamond_f({ x, y });
				}
				offset = !offset;
			}
		}

		void square_step(const RNG & rng, Matrix<float> & grid, size_t radius)
		{
			auto get_square_f = [&grid, radius](grid_pt center) { return get_square(grid, radius, center); };
			auto square_f = [&](grid_pt && center) { return aggregate(get_square_f, rng, std::forward<grid_pt>(center)); };

			for (size_t x = radius; x < grid.width(); x += (2 * radius))
			{
				for (size_t y = radius; y < grid.height(); y += (2 * radius))
				{
					grid.at(x, y) = square_f({ x, y });
				}
			}
		}

		RNG::result_type aggregate(const std::function<step_pts(grid_pt)> & pts, const RNG & rng, grid_pt center)
		{
			auto targets = pts(center);
			auto val = rng();
			auto avg = average(targets);
			return avg + val;
		}

		step_pts get_diamond(const Matrix<float> & grid, size_t radius, grid_pt center)
		{
			auto[x, y] = center;

			auto up     = grid_get_safe(grid, x         , y - radius);
			auto right  = grid_get_safe(grid, x + radius, y         );
			auto bottom = grid_get_safe(grid, x         , y + radius);
			auto left   = grid_get_safe(grid, x - radius, y         );

			return { up, right, bottom, left };
		}

		step_pts get_square(const Matrix<float> & grid, size_t radius, grid_pt center)
		{
			auto[x, y] = center;

			auto tl = grid_get_safe(grid, x - radius, y - radius);
			auto tr = grid_get_safe(grid, x + radius, y - radius);
			auto br = grid_get_safe(grid, x + radius, y + radius);
			auto bl = grid_get_safe(grid, x - radius, y + radius);

			return { tl, tr, br, bl };
		}

		std::optional<float> grid_get_safe(const Matrix<float> & grid, size_t x, size_t y)
		{
			if (x < 0 || x >= grid.width())
			{
				return std::nullopt;
			}

			if (y < 0 || y >= grid.height())
			{
				return std::nullopt;
			}

			return std::make_optional(grid[grid.index(static_cast<std::size_t>(x), static_cast<std::size_t>(y))]);
		}
	}
}
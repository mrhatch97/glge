#pragma once

#include <glge/Common.h>
#include <glge/Util/Util.h>

#include <random>
#include <tuple>

namespace eng
{
	namespace procgen
	{
		[[nodiscard]] util::Matrix<float> diamond_square_gen(std::seed_seq & seed,
			size_t n, float variation, float top_left, float top_right, float bottom_left, float bottom_right);
	}
}
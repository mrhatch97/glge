#pragma once

#include <glge/common.h>
#include <glge/util/util.h>

#include <random>
#include <tuple>

namespace glge::proc_gen
{
		[[nodiscard]] util::Matrix<float> diamond_square_gen(std::seed_seq & seed,
			size_t n, float variation, float top_left, float top_right, float bottom_left, float bottom_right);
}

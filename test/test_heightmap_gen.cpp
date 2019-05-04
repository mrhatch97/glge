#include <glge/support/proc_gen/terrain.h>

#include "test_utils.h"

namespace glge::test
{
	using namespace glge::proc_gen;

	void test_gen()
	{
		float variation = 10.0f;
		auto heightmap = diamond_square_gen(test_rand_seed, 10, variation, 0.0f,
											5.0f, 10.0f, -5.0f);

		test_assert(heightmap.at(0, 0) == 0.0f,
					"Top left corner value changed");
		test_assert(heightmap.at(heightmap.width() - 1, 0) == 5.0f,
					"Top right corner value changed");
		test_assert(heightmap.at(0, heightmap.height() - 1) == 10.0f,
					"Bottom left corner value changed");
		test_assert(heightmap.at(heightmap.width() - 1,
								 heightmap.height() - 1) == -5.0f,
					"Bottom right corner value changed");

		// Expect 9 iterations with n = 10; each iteration adds at most
		// variation and subtracts at most variation min(corners) - variation *
		// 9, max(corners) + variation * 9
		auto in_expected_range = [](float val) {
			return val >= -95.0f && val <= 100.0f;
		};

		bool all_in_range = true;
		for (std::size_t i = 0; i < heightmap.width(); i++)
		{
			for (std::size_t j = 0; j < heightmap.height(); j++)
			{
				all_in_range &= in_expected_range(heightmap.at(i, j));
			}
		}
		test_assert(all_in_range, "Generated value was outside expected range");
	}

	void test_bad()
	{
		float variation = -10.0f;   // disallowed
		test_fails(
			[=] {
				(void)diamond_square_gen(test_rand_seed, 10, variation, 0.0f,
										 0.0f, 0.0f, 0.0f);
			},
			"Variation allowed to be negative");
	}
}   // namespace glge::test

using namespace glge::test;

int main()
{
	Test(test_gen).run();
	Test(test_bad).run();
}

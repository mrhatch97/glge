#include <glge/util/math.h>

#include "test_utils.h"

namespace glge::test::cases
{
	using namespace glge::math;

	/// \test Tests the functionality of glge::math::wrap_range in several
	/// cases.
	void test_wrap()
	{
		test_equal(0, wrap_range(0, -5, 5));
		test_equal(-3, wrap_range(7, -5, 5));
		test_equal(1, wrap_range(-9, -5, 5));
		test_equal(1, wrap_range(11, 0, 5));
		test_equal(4, wrap_range(-11, 0, 5));
	}
}   // namespace glge::test::cases

int main()
{
	using glge::test::Test;
	using namespace glge::test::cases;

	Test::run(test_wrap);
}

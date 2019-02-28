#include <glge/Util/Math.h>

#include "test_utils.h"

using namespace eng;
using namespace eng::math;

int main()
{
	test_equal(0, wrap_range(0, -5, 5));
	test_equal(-3, wrap_range(7, -5, 5));
	test_equal(1, wrap_range(-9, -5, 5));
	test_equal(1, wrap_range(11, 0, 5));
	test_equal(4, wrap_range(-11, 0, 5));
}
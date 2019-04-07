#include <glge/util/math.h>

#include "test_utils.h"

using namespace glge;
using namespace glge::math;

void test_wrap()
{
	test_equal(0, wrap_range(0, -5, 5));
	test_equal(-3, wrap_range(7, -5, 5));
	test_equal(1, wrap_range(-9, -5, 5));
	test_equal(1, wrap_range(11, 0, 5));
	test_equal(4, wrap_range(-11, 0, 5));
}

int main()
{
	Test(test_wrap).run();
}
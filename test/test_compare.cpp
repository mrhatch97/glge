#include <glge/util/math.h>

#include "test_utils.h"

#include <cmath>

using namespace glge;
using namespace glge::math;

void test_compare()
{
	vec3 v1(3.0f, 2.0f, 6.0f);
	vec3 v2(2.0f, 6.0f, 3.0f);

	test_assert(compare_by_x(v2, v1));

	v2 = vec3(4.0f, 6.0f, 3.0f);

	test_assert(!compare_by_x(v2, v1));

	test_assert(!compare_by_y(v2, v1));

	v2 = vec3(2.0f, -6.0f, 3.0f);

	test_assert(compare_by_y(v2, v1));

	test_assert(compare_by_z(v2, v1));

	v2 = vec3(2.0f, 1.0f, 7.0f);

	test_assert(!compare_by_z(v2, v1));
}

void test_compare_abs()
{
	vec3 v1(-3.0f, 2.0f, -6.0f);
	vec3 v2(2.0f, -6.0f, 3.0f);

	test_assert(compare_by_x_abs(v2, v1));

	v2 = vec3(-4.0f, -6.0f, 3.0f);

	test_assert(!compare_by_x_abs(v2, v1));

	test_assert(!compare_by_y_abs(v2, v1));

	v2 = vec3(2.0f, -1.0f, 3.0f);

	test_assert(compare_by_y_abs(v2, v1));

	test_assert(compare_by_z_abs(v2, v1));

	v2 = vec3(2.0f, -6.0f, 7.0f);

	test_assert(!compare_by_z_abs(v2, v1));
}

void test_compare_mag()
{
	vec3 v1(-3.0f, 2.0f, -7.0f);
	vec3 v2(2.0f, -6.0f, 3.0f);

	test_assert(compare_by_magnitude(v2, v1));

	v2 = vec3(4.0f, 7.0f, -9.0f);

	test_assert(!compare_by_magnitude(v2, v1));
}

int main()
{
	Test(test_compare).run();
	Test(test_compare_abs).run();
	Test(test_compare_mag).run();
}
#include <glge/util/math.h>

#include "test_utils.h"

namespace glge::test::cases
{
	using namespace glge::math;

	/// \test Tests that the computation of a trackball point in the middle
	/// of the window results in the expected point.
	void test_trackball_center()
	{
		const vec3 result = trackball_point(600.0f, 600.0f, 300.0f, 300.0f);

		const vec3 expected(0.0f, 0.0f, 1.0f);

		test_assert(vec_eq(result, expected), "Result was not correct");
	}

	/// \test Tests that the computation of a trackball point at the right
	/// side of the window results in the expected point.
	void test_trackball_right()
	{
		const vec3 result = trackball_point(600.0f, 600.0f, 600.0f, 300.0f);

		const vec3 expected(1.0f, 0.0f, 0.0f);

		test_assert(vec_eq(result, expected), "Result was not correct");
	}

	/// \test Tests that the computation of a trackball point at the top
	/// side of the window results in the expected point.
	void test_trackball_up()
	{
		const vec3 result = trackball_point(600.0f, 600.0f, 300.0f, 0.0f);

		const vec3 expected(0.0f, 1.0f, 0.0f);

		test_assert(vec_eq(result, expected), "Result was not correct");
	}

	/// \test Tests that the type-safe glm trigonometric function wrappers 
	/// function as expected.
	void test_trig()
	{
		// Sanity check - this will fail if for some unholy reason GLM
		// is defaulting to degrees
		test_assert(float_eq(0.0f, glm::cos(glm::radians(90.0f))));

		test_assert(float_eq(1.0f, sin(Degrees(90.0f))));
		test_assert(float_eq(1.0f, cos(Degrees(0.0f))));
		test_assert(float_eq(1.0f, tan(Degrees(45.0f))));
	}
}   // namespace glge::test::cases


int main()
{
	using glge::test::Test;
	using namespace glge::test::cases;

	Test::run(test_trackball_center);
	Test::run(test_trackball_right);
	Test::run(test_trackball_up);
	Test::run(test_trig);
}

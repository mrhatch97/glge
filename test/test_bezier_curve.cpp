#include <glge/util/math.h>

#include "test_utils.h"

#include <cmath>

namespace glge::test
{
	using namespace glge::math;

	void test_eval()
	{
		BezierCurve curve(vec3(0, 0, 0), vec3(0, 4.0f, 0), vec3(6.0f, 4.0f, 0),
						  vec3(6.0f, 0, 0));

		auto result = curve.evaluate_at(0.50f, BezierCurve::value_polynomial);

		test_assert(vec3_eq(vec3(3.0f, 3.0f, 0.0f), result),
					"Result was incorrect");
	}

	void test_eval_bad()
	{
		BezierCurve curve(vec3(0, 0, 0), vec3(0, 4.0f, 0), vec3(6.0f, 4.0f, 0),
						  vec3(6.0f, 0, 0));

		test_fails([&curve] {
			curve.evaluate_at(1.50f, BezierCurve::value_polynomial);
		});
		test_fails([&curve] {
			curve.evaluate_at(-.50f, BezierCurve::value_polynomial);
		});
	}

	void test_path_eval()
	{
		BezierPath path{{{vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -4.0f, 0.0f)},
						 {vec3(6.0f, 0.0f, 0.0f), vec3(6.0f, 4.0f, 0.0f)}}};

		auto result_1 = path.evaluate_at(0.25f, BezierCurve::value_polynomial);
		auto result_2 = path.evaluate_at(0.75f, BezierCurve::value_polynomial);

		test_assert(vec3_eq(vec3(3.0f, 3.0f, 0.0f), result_1),
					"Path result_1 was incorrect");

		test_assert(vec3_eq(vec3(3.0f, -3.0f, 0.0f), result_2),
					"Path result_2 was incorrect");
	}

	void test_path_eval_bad()
	{
		BezierPath path{{{vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -4.0f, 0.0f)},
						 {vec3(6.0f, 0.0f, 0.0f), vec3(6.0f, 4.0f, 0.0f)}}};

		test_fails([&path] {
			path.evaluate_at(1.50f, BezierCurve::value_polynomial);
		});
		test_fails([&path] {
			path.evaluate_at(-.50f, BezierCurve::value_polynomial);
		});
	}
}   // namespace glge::test

using namespace glge::test;

int main()
{
	Test(test_eval).run();
	Test(test_eval_bad).run();
	Test(test_path_eval).run();
	Test(test_path_eval_bad).run();
}

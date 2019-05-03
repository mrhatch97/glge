#include <glge/util/math.h>

#include "test_utils.h"

#include <cmath>

using namespace glge;
using namespace glge::math;

static bool fp_equal(float a, float b, float moe)
{
  return std::abs(a - b) <= moe;
}

static bool fp_equal(vec3 a, vec3 b, float moe)
{
	return fp_equal(a.x, b.x, moe) && fp_equal(a.y, b.y, moe) &&
		   fp_equal(a.z, b.z, moe);
}

void test_eval()
{
	BezierCurve curve(vec3(0, 0, 0), vec3(0, 4.0f, 0), vec3(6.0f, 4.0f, 0),
					  vec3(6.0f, 0, 0));

	auto result = curve.evaluate_at(0.50f, BezierCurve::value_polynomial);

	test_assert(fp_equal(vec3(3.0f, 3.0f, 0.0f), result, 0.001f),
				"Result was incorrect");
}

void test_eval_bad() 
{
	BezierCurve curve(vec3(0, 0, 0), vec3(0, 4.0f, 0), vec3(6.0f, 4.0f, 0),
					  vec3(6.0f, 0, 0));

	test_fails(
		[&curve] { curve.evaluate_at(1.50f, BezierCurve::value_polynomial); });
	test_fails(
		[&curve] { curve.evaluate_at(-.50f, BezierCurve::value_polynomial); });
}

void test_path_eval()
{
	BezierPath path{{
		{vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -4.0f, 0.0f)},
		{vec3(6.0f, 0.0f, 0.0f), vec3(6.0f, 4.0f, 0.0f)}
	}};

  auto result_1 = path.evaluate_at(0.25f, BezierCurve::value_polynomial);
  auto result_2 = path.evaluate_at(0.75f, BezierCurve::value_polynomial);

	test_assert(fp_equal(vec3(3.0f, 3.0f, 0.0f), result_1, 0.001f),
				"Path result_1 was incorrect");

	test_assert(fp_equal(vec3(3.0f, -3.0f, 0.0f), result_2, 0.001f),
				"Path result_2 was incorrect");
}

void test_path_eval_bad()
{
	BezierPath path{{
		{vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -4.0f, 0.0f)},
		{vec3(6.0f, 0.0f, 0.0f), vec3(6.0f, 4.0f, 0.0f)}
	}};

	test_fails(
		[&path] { path.evaluate_at(1.50f, BezierCurve::value_polynomial); });
	test_fails(
		[&path] { path.evaluate_at(-.50f, BezierCurve::value_polynomial); });
}

int main()
{
	Test(test_eval).run();
	Test(test_eval_bad).run();
	Test(test_path_eval).run();
	Test(test_path_eval_bad).run();
}

#include <glge/util/math.h>

#include "test_utils.h"

#include <cmath>

namespace glge::test::cases
{
	using namespace glge::math;

    /// <summary>
    /// Context for Bezier curve tests.
    /// </summary>
	class BezierTest : public Test
	{
	public:
		/// \test Tests whether a BezierCurve is correctly evaluated under
		/// normal conditions.
		void test_eval()
		{
			BezierCurve curve(vec3(0, 0, 0), vec3(0, 4.0f, 0),
							  vec3(6.0f, 4.0f, 0), vec3(6.0f, 0, 0));

			auto result =
				curve.evaluate_at(0.50f, BezierCurve::value_polynomial);

			test_assert(vec_eq(vec3(3.0f, 3.0f, 0.0f), result),
						"Result was incorrect");
		}

		/// \test Tests whether a BezierCurve rejects evaluation at invalid
		/// values of t.
		void test_eval_bad()
		{
			BezierCurve curve(vec3(0, 0, 0), vec3(0, 4.0f, 0),
							  vec3(6.0f, 4.0f, 0), vec3(6.0f, 0, 0));

			test_fails([&curve] {
				curve.evaluate_at(1.50f, BezierCurve::value_polynomial);
			});
			test_fails([&curve] {
				curve.evaluate_at(-.50f, BezierCurve::value_polynomial);
			});
		}

		/// \test Tests whether a BezierPath is correctly evaluated under normal
		/// conditions.
		void test_path_eval()
		{
			BezierPath path{{{vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -4.0f, 0.0f)},
							 {vec3(6.0f, 0.0f, 0.0f), vec3(6.0f, 4.0f, 0.0f)}}};

			auto result_1 =
				path.evaluate_at(0.25f, BezierCurve::value_polynomial);
			auto result_2 =
				path.evaluate_at(0.75f, BezierCurve::value_polynomial);

			test_assert(vec_eq(vec3(3.0f, 3.0f, 0.0f), result_1),
						"Path result_1 was incorrect");

			test_assert(vec_eq(vec3(3.0f, -3.0f, 0.0f), result_2),
						"Path result_2 was incorrect");
		}

		/// \test Tests whether a BezierPath rejects evaluation at invalid
		/// values of t.
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
	};

}   // namespace glge::test

int main()
{
    using glge::test::Test;
    using glge::test::cases::BezierTest;

    Test::run(&BezierTest::test_eval);
    Test::run(&BezierTest::test_eval_bad);
    Test::run(&BezierTest::test_path_eval);
    Test::run(&BezierTest::test_path_eval_bad);
}

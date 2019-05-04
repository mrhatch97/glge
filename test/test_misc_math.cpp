#include <glge/util/math.h>

#include "test_utils.h"

namespace glge::test
{
	using namespace glge::math;

	void test_trackball_center() 
  {
    const vec3 result = trackball_point(600.0f, 600.0f, 300.0f, 300.0f);

    const vec3 expected(0.0f, 0.0f, 1.0f);

    test_assert(vec_eq(result, expected), "Result was not correct");
  }

	void test_trackball_right() 
  {
    const vec3 result = trackball_point(600.0f, 600.0f, 600.0f, 300.0f);

    const vec3 expected(1.0f, 0.0f, 0.0f);

    test_assert(vec_eq(result, expected), "Result was not correct");
  }

	void test_trackball_up() 
  {
    const vec3 result = trackball_point(600.0f, 600.0f, 300.0f, 0.0f);

    const vec3 expected(0.0f, 1.0f, 0.0f);

    test_assert(vec_eq(result, expected), "Result was not correct");
  }
}   // namespace glge::test

using namespace glge::test;

int main()
{
	Test(test_trackball_center).run();
  Test(test_trackball_right).run();
  Test(test_trackball_up).run();
}

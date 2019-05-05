#include <glge/util/math.h>

#include "test_utils.h"

namespace glge::test::cases
{
	using namespace glge::math;

	/// \test Tests that a Stepper with a step of 0 does nothing.
	void test_0_step()
	{
		Stepper<int> stepper(3, 0);

		stepper++;
		stepper++;

		test_equal(3, stepper);

		stepper--;

		test_equal(3, stepper);
	}

	/// \test Tests that a Stepper with a positive step correctly increments
	/// and decrements.
	void test_positive_step()
	{
		Stepper<int> stepper(0, 3);

		stepper++;
		stepper++;

		test_equal(6, stepper);

		stepper--;
		stepper--;
		stepper--;

		test_equal(-3, stepper);
	}

	/// \test Tests that a Stepper with a negative step correctly increments
	/// and decrements.
	void test_negative_step()
	{
		Stepper<int> stepper(0, -3);

		stepper++;
		stepper++;

		test_equal(-6, stepper);

		stepper--;
		stepper--;
		stepper--;

		test_equal(3, stepper);
	}
}   // namespace glge::test::cases

int main()
{
	using glge::test::Test;
	using namespace glge::test::cases;

	Test::run(test_0_step);
	Test::run(test_positive_step);
	Test::run(test_negative_step);
}

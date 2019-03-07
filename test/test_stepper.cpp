#include <glge/util/math.h>

#include "test_utils.h"

using namespace glge;
using namespace glge::math;

int main()
{
	{
		Stepper<int> stepper(3, 0);

		stepper++;
		stepper++;

		test_equal(3, stepper);

		stepper--;

		test_equal(3, stepper);
	}

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
}
#include <glge/util/math.h>

#include "test_utils.h"

using namespace glge;
using namespace glge::math;

void test_0_step()
{
	Stepper<int> stepper(3, 0);

	stepper++;
	stepper++;

	test_equal(3, stepper);

	stepper--;

	test_equal(3, stepper);
}

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

int main()
{
	Test(test_0_step).run();
	Test(test_positive_step).run();
	Test(test_negative_step).run();
}
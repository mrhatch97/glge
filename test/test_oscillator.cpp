#include <glge/util/math.h>

#include "test_utils.h"

using namespace glge;
using namespace glge::math;

int main()
{
	{
		Oscillator<int> o(0, 2, -6, 6);

		for (int i = 0; i < 14; i++)
		{
			o++;
		}

		test_equal(4, o);
	}

	{
		Oscillator<int> o(0, 2, -6, 6);

		for (int i = 0; i < 14; i++)
		{
			o--;
		}

		test_equal(-4, o);
	}

	TEST_FAILS(
		Oscillator<int> o(0, 13, -6, 6);
	);

	TEST_FAILS(
		Oscillator<int> o(0, 2, 6, -6);
	);
}
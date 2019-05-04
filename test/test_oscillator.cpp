#include <glge/util/math.h>

#include "test_utils.h"

namespace glge::test
{
	using namespace glge::math;

	void test_increment()
	{
		Oscillator<int> o(0, 2, -6, 6);

		for (int i = 0; i < 14; i++)
		{
			o++;
		}

		test_equal(4, o);
	}

	void test_decrement()
	{
		Oscillator<int> o(0, 2, -6, 6);

		for (int i = 0; i < 14; i++)
		{
			o--;
		}

		test_equal(-4, o);
	}

	void test_invalid()
	{
		test_fails([] { Oscillator<int> o(0, 13, -6, 6); });
		test_fails([] { Oscillator<int> o(0, 2, 6, -6); });
	}

}   // namespace glge::test

using namespace glge::test;

int main()
{
	Test(test_increment).run();
	Test(test_decrement).run();
	Test(test_invalid).run();
}

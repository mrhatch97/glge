#include <glge/util/math.h>

#include "test_utils.h"

namespace glge::test::cases
{
	using namespace glge::math;

	/// \test Tests that an Oscillator correctly changes value when incremented.
	void test_increment()
	{
		Oscillator<int> o(0, 2, -6, 6);

		for (int i = 0; i < 14; i++)
		{
			o++;
		}

		test_equal(4, o);
	}

	/// \test Tests that an Oscillator correctly changes value when decremented.
	void test_decrement()
	{
		Oscillator<int> o(0, 2, -6, 6);

		for (int i = 0; i < 14; i++)
		{
			o--;
		}

		test_equal(-4, o);
	}

	/// \test Tests that Oscillator rejects too-large deltas and reversed
	/// ranges.
	void test_invalid()
	{
		test_fails([] { Oscillator<int> o(0, 13, -6, 6); });
		test_fails([] { Oscillator<int> o(0, 2, 6, -6); });
	}

}   // namespace glge::test::cases

int main()
{
	using glge::test::Test;
	using namespace glge::test::cases;

	Test::run(test_increment);
	Test::run(test_decrement);
	Test::run(test_invalid);
}

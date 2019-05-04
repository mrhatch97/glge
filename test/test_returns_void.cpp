#include <glge/util/util.h>

#include "test_utils.h"

namespace glge::test
{
	using namespace glge::util;

	/// \test Tests whether returns_void correctly identifies the return type of
	/// lambda expressions.
	void test_returns()
	{
        auto f_1 = [] { return; };
        auto f_2 = [] { return 1; };

		static_assert(returns_void_v<decltype(f_1)>);

		static_assert(!returns_void_v<decltype(f_2)>);
	}
}   // namespace glge::test

using namespace glge::test;

int main()
{
	Test(test_returns).run();
}

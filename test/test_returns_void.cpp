#include <internal/util/util.h>

#include "test_utils.h"

namespace glge::test::cases
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
}   // namespace glge::test::cases


int main()
{
    using glge::test::Test;
	using namespace glge::test::cases;

	Test::run(test_returns);
}

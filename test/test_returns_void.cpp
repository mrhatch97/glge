#include <glge/util/util.h>

using namespace glge::util;

int main()
{
	auto f_1 = [] { return; };
	auto f_2 = [] { return 1; };

	static_assert(returns_void_v<decltype(f_1)>);

	static_assert(!returns_void_v<decltype(f_2)>);

	return 0;
}
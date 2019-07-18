#include <internal/util/util.h>

#include "test_utils.h"

namespace glge::test::cases
{
	using namespace glge::util;

    /// \test Tests whether safe casts are not flagged as failing
    /// by glge::util::safe_cast.
	void test_successful()
	{
		safe_cast<float, double>(3.14f);
		safe_cast<float, float>(3.14f);
	}

    /// \test Tests whether unsafe casts are flagged as failing
    /// by glge::util::safe_cast.
	void test_failure()
	{
		test_fails([] { safe_cast<double, int>(3.14); });
		test_fails([] { safe_cast<double, float>(3.14); });
	}
}   // namespace glge::test::cases


int main()
{
	using glge::test::Test;
	using namespace glge::test::cases;

    Test::run(test_successful);
    Test::run(test_failure);
}

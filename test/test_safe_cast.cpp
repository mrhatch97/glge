#include <glge/util/util.h>

#include "test_utils.h"

using namespace glge::util;

void test_successful()
{
	safe_cast<float, double>(3.14f);
	safe_cast<float, float>(3.14f);
}

void test_failure()
{
	test_fails([] { safe_cast<double, int>(3.14); });
	test_fails([] { safe_cast<double, float>(3.14); });
}

int main()
{
	Test(test_successful).run();
	Test(test_failure).run();
}
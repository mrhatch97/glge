#pragma once

#include <iostream>

constexpr int TEST_FAIL_CODE = -1;

#define TEST_FAILS(inner) try\
{\
inner; \
std::exit(TEST_FAIL_CODE);\
}\
catch(const std::logic_error &)\
{ }

inline void test_assert(bool val, std::string msg = "")
{
	if (!val)
	{
		std::cerr << msg << std::endl;
		std::exit(TEST_FAIL_CODE);
	}
}

template<typename E, typename A>
void test_equal(const E & expected, const A & actual)
{
	if (expected != actual)
	{
		std::cerr << "Expected: " << expected << ", but got: " << actual << std::endl;
		std::exit(TEST_FAIL_CODE);
	}
}
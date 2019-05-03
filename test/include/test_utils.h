#pragma once

#include <glge/util/util.h>

#include <cstdint>
#include <iostream>
#include <sstream>

constexpr int test_fail_code = -1;
constexpr std::uint32_t test_rand_seed = 1754195816U;

constexpr float fp_epsilon = 0.001f;

static bool vec3_eq(glm::vec3 a, glm::vec3 b)
{
	return glm::all(glm::lessThan(glm::abs(a - b), glm::vec3(fp_epsilon)));
}

template<typename T = void (*)()>
class Test
{
private:
	T test;

	void pre()
	{
		try
		{
			pre_test();
		}
		catch (const std::exception & e)
		{
			std::cerr << "Test preconditions failed: " << std::endl;
			glge::util::print_nested_exception(e);
			std::exit(test_fail_code);
		}
	}

	void post()
	{
		try
		{
			post_test();
		}
		catch (const std::exception & e)
		{
			std::cerr << "WARNING: Test post step failed: " << std::endl;
			glge::util::print_nested_exception(e);
		}
	}

protected:
	virtual void pre_test(){};
	virtual void post_test(){};

public:
	Test(T test) : test(test) {}

	void run()
	{
		pre();
		try
		{
			test();
		}
		catch (const std::exception & e)
		{
			glge::util::print_nested_exception(e);
			std::exit(test_fail_code);
		}
		post();
	}

	virtual ~Test() = default;
};

template<typename T>
Test(T test)->Test<T>;

template<typename F>
inline void test_fails(F f, std::string msg = "Expected exception")
{
	try
	{
		f();
		throw std::runtime_error(EXC_MSG(msg));
	}
	catch (const std::logic_error &)
	{
	}
}

inline void test_assert(bool val, std::string msg = "Assertion failed")
{
	if (!val)
	{
		throw std::runtime_error(EXC_MSG(msg));
	}
}

template<typename E, typename A>
void test_equal(const E & expected, const A & actual)
{
	if (expected != actual)
	{
		std::ostringstream stream;
		stream << "Expected: " << expected << ", but got: " << actual;
		throw std::runtime_error(EXC_MSG(stream.str()));
	}
}

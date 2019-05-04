#pragma once

#include <glge/common.h>
#include <glge/util/util.h>

#include <cstdint>
#include <iostream>
#include <sstream>

namespace glge::test
{
	/// <summary>Exit code returned when a test process fails.</summary>
	constexpr int test_fail_code = -1;

	/// <summary>
	/// Fixed random seed for testing functions requiring randomness.
	/// </summary>
	constexpr std::uint32_t test_rand_seed = 1754195816U;

	/// <summary>
	/// Margin of error for floating point equality checks in tests.
	/// </summary>
	constexpr float fp_epsilon = 0.001f;

	/// <summary>
	/// Tests whether two glm vectors are equal within fp_epsilon margin of
	/// error.
	/// </summary>
	/// <typeparam name="VecT">
	/// Type of glm vector, e.g. glm::vec3.
	/// </typeparam>
	/// <param name="a">
	/// First vector to check.
	/// </param>
	/// <param name="b">
	/// Second vector to check.
	/// </param>
	/// <returns>True if vectors are approximately equal.</returns>
	template<typename VecT>
	static bool vec_eq(VecT a, VecT b)
	{
		return glm::all(glm::epsilonEqual(a, b, fp_epsilon));
	}

	/// <summary>
	/// Tests whether two glm mat4 are equal within fp_epsilon margin of
	/// error.
	/// </summary>
	/// <param name="a">
	/// First matrix to check.
	/// </param>
	/// <param name="b">
	/// Second matrix to check.
	/// </param>
	/// <returns>True if matrices are approximately equal.</returns>
	static bool mat4_eq(glm::mat4 a, glm::mat4 b)
	{
		for (int i = 0; i < 4; i++)
		{
			if (!vec_eq(a[i], a[i]))
			{
				return false;
			}
		}

		return true;
	}

	/// <summary>
	/// Helper class for executing a test function with optional pre and post
	/// conditions.
	/// </summary>
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
        /// <summary>
        /// Function to run before the main test.
        /// </summary>
		virtual void pre_test(){};

        /// <summary>
        /// Function to run after the main test.
        /// </summary>
		virtual void post_test(){};

	public:
        /// <summary>
        /// Construct a new Test wrapping the given callable test function.
        /// </summary>
        /// <param name="test">
        /// Callable object to be invoked as the test function.
        /// </param>
		Test(T test) : test(test) {}

        /// <summary>
        /// Run the pre-test, test, and post-test. std::exit is invoked if an
        /// exception is thrown.
        /// </summary>
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

        /// <summary>
        /// Default destructor.
        /// </summary>
		virtual ~Test() = default;
	};

	/// <summary>
	/// Deduction guide for inferring the callable type T in Test.
	/// </summary>
	template<typename T>
	Test(T test)->Test<T>;

	/// <summary>
	/// Asserts that the given function, when executed, throws std::logic_error.
	/// If it does not, throws std::runtime_error with the provided explanation
	/// message or a default.
	/// </summary>
	/// <typeparam name="F">
	/// Callable type of passed function.
	/// </typeparam>
	/// <param name="f">
	/// Callable object to invoke.
	/// </param>
	/// <param name="msg">
	/// Explanatory message to attach to thrown exception.
	/// </param>
	template<typename F>
	void test_fails(F f,
					std::string msg = "Expected exception std::logic_error")
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

	/// <summary>
	/// Asserts that the given boolean is true. If not, throws
	/// std::runtime_error with the provided explanation message or a default.
	/// </summary>
	/// <param name="val">
	/// Boolean value to assert.
	/// </param>
	/// <param name="msg">
	/// Explanatory message to attach to thrown exception.
	/// </param>
	void test_assert(bool val, std::string msg = "Assertion failed")
	{
		if (!val)
		{
			throw std::runtime_error(EXC_MSG(msg));
		}
	}

	/// <summary>
	/// Asserts that the given values are equal. If not, throws
	/// std::runtime_error with the expected and actual values
	/// in the explanation message.
	/// </summary>
	/// <typeparam name="E">
	/// Type of expected value. Must implement operator<< for std::ostream.
	/// </typeparam>
	/// <typeparam name="A">
	/// Type of actual value. Must implement operator<< for std::ostream.
	/// </typeparam>
	/// <param name="expected">
	/// Expected value.
	/// </param>
	/// <param name="actual">
	/// Actual value.
	/// </param>
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
}   // namespace glge::test

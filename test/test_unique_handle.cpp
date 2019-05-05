#include <glge/util/util.h>

#include "test_utils.h"

namespace glge::test::cases
{
	using namespace glge::util;

	/// \test Tests that a UniqueHandle correctly invokes its entry and exit
	/// functions.
	void test_basic()
	{
		bool enter = false, exit = false;

		{
			UniqueHandle handle([&] { enter = true; }, [&] { exit = true; });
			test_assert(handle);
			test_assert(enter);
			test_assert(!exit);
		}
		test_assert(exit);
	}

	/// \test Tests that a UniqueHandle invokes chained exit functions in
	/// the correct order.
	void test_chain()
	{
		int enter = 0, exit = 0;

		{
			UniqueHandle handle([&] { test_equal(0, enter++); },
								[&] { test_equal(1, exit++); });

			test_equal(1, enter);
			test_equal(0, exit);

			handle.chain([&] { test_equal(1, enter++); },
						 [&] { test_equal(0, exit++); });

			test_equal(2, enter);
			test_equal(0, exit);
		}

		test_equal(2, exit);
	}

	/// \test Tests that a UniqueHandle invokes its exit function immediately
	/// when reset is invoked.
	void test_reset()
	{
		bool enter = false, exit = false;

		UniqueHandle handle([&] { enter = true; }, [&] { exit = true; });

		handle.reset();

		test_assert(exit);
		test_assert(!handle);
	}

	/// \test Tests that a UniqueHandle invokes its exit function immediately
	/// when reset is invoked, and properly manages the replacement function.
	void test_reset_f()
	{
		bool enter = false, exit = false, exit_2 = false;

		UniqueHandle handle([&] { enter = true; }, [&] { exit = true; });

		handle.reset([&] { exit_2 = true; });

		test_assert(exit);
		test_assert(!exit_2);
		test_assert(handle);

		handle.reset();

		test_assert(exit_2);
	}

	/// \test Tests that a UniqueHandle does not invoke its exit function
	/// when release is invoked.
	void test_release()
	{
		bool enter = false, exit = false;

		{
			UniqueHandle handle([&] { enter = true; }, [&] { exit = true; });

			handle.release();

			test_assert(!handle);
		}

		test_assert(!exit);
	}

    /// \test Tests that a UniqueHandle properly transfers its exit
    /// responsibility when moved.
	void test_move()
	{
		bool enter = false, exit = false;

		{
			UniqueHandle handle_1;

			{
				UniqueHandle handle_2([&] { enter = true; },
									  [&] { exit = true; });

				handle_1 = std::move(handle_2);
			}
			test_assert(!exit);
		}
		test_assert(exit);
	}
}   // namespace glge::test::cases


int main()
{
	using glge::test::Test;
	using namespace glge::test::cases;

	Test::run(test_basic);
	Test::run(test_chain);
	Test::run(test_reset);
	Test::run(test_reset_f);
	Test::run(test_release);
	Test::run(test_move);
}

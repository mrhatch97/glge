#include <glge/support/event/event.h>

#include "test_utils.h"

namespace glge::test
{
	using namespace glge::event;

	static const int test_val = 30;

	void test_basic()
	{
		bool success = false;
		Event<int> event;

		event.register_handler([&](int arg) {
			if (arg == test_val)
			{
				success = true;
			}
		});

		event.raise(30);

		test_assert(success, "Value was not set");
	}

	void test_unsubscribe()
	{
		bool success = true;
		Event<int> event;

		auto subscription = event.register_handler([&](int arg) {
			if (arg == test_val)
			{
				success = false;
			}
		});

		subscription.release();

		event.raise(30);

		test_assert(success, "Event was not unsubscribed");
	}

	void test_multiple()
	{
		int call_count = 0;
		Event<int> event;

		auto handler = [&](int arg) {
			if (arg == test_val)
			{
				call_count++;
			}
		};

		auto sub1 = event.register_handler(handler);
		event.register_handler(handler);
		auto sub3 = event.register_handler(handler);
		event.register_handler(handler);
		event.register_handler(handler);

		sub1.release();
		sub3.release();

		event.raise(test_val);

		test_equal(3, call_count);
	}
}   // namespace glge::test

using namespace glge::test;

int main()
{
	Test(test_basic).run();
	Test(test_unsubscribe).run();
	Test(test_multiple).run();
}

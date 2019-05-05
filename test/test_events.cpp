#include <glge/support/event/event.h>

#include "test_utils.h"

namespace glge::test::cases
{
	using namespace glge::event;

	static const int test_val = 30;

	/// <summary>
	/// Test context for Event tests.
	/// </summary>
	class EventTest : public Test
	{
	private:
		Event<int> event;

	public:
		/// \test Tests whether an event properly raises a single handler.
		void test_basic()
		{
			bool success = false;

			event.register_handler([&](int arg) {
				if (arg == test_val)
				{
					success = true;
				}
			});

			event.raise(30);

			test_assert(success, "Value was not set");
		}

		/// \test Tests whether an Event does not raise an unsubscribed handler.
		void test_unsubscribe()
		{
			bool success = true;

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

		/// \test Tests whether an Event properly raises multiple handlers and
		/// does not raise unsubscribed handlers.
		void test_multiple()
		{
			int call_count = 0;

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
	};
}   // namespace glge::test::cases

int main()
{
	using glge::test::Test;
	using glge::test::cases::EventTest;

	Test::run(&EventTest::test_basic);
	Test::run(&EventTest::test_unsubscribe);
	Test::run(&EventTest::test_multiple);
}

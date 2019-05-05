#include <glge/support/event/input/input_dispatcher.h>

#include "test_utils.h"

namespace glge::test::cases
{
	using namespace glge::event::input;

	/// <summary>Context for input tests.</summary>
	class InputTest : public Test
	{
	private:
		InputDispatcher dispatcher;

	public:
		/// \test Tests that InputDispatcher raises the key handler correctly.
		void test_single_key()
		{
			int recv_keys = 0;

			const LogicalKey sent_key = KEY_J;

			dispatcher.key_press.register_handler([&](KeyInput input) {
				if (input.key == sent_key)
				{
					recv_keys++;
				}
			});

			dispatcher.notify_key_input(PRESS, sent_key);
			dispatcher.notify_key_input(RELEASE, sent_key);

			test_equal(1, recv_keys);
		}

		/// \test Tests that InputDispatcher raises the key handler correctly
		/// for repeated polled inputs.
		void test_repeat_key()
		{
			int recv_keys = 0;

			const LogicalKey sent_key = KEY_J;

			dispatcher.key_press.register_handler([&](KeyInput input) {
				if (input.key == sent_key)
				{
					recv_keys++;
				}
			});

			dispatcher.notify_key_input(PRESS, sent_key);
			dispatcher.poll_key_inputs();
			dispatcher.poll_key_inputs();
			dispatcher.notify_key_input(RELEASE, sent_key);
			dispatcher.poll_key_inputs();

			test_equal(3, recv_keys);
		}

		/// \test Tests that InputDispatcher raises the mouse button handler
		/// correctly.
		void test_mouse_button()
		{
			int recv_down = 0;
			int recv_up = 0;

			const LogicalButton sent_button = BUTTON_RIGHT;

			dispatcher.mouse_down.register_handler([&](MouseDown input) {
				if (input.button == sent_button)
				{
					recv_down++;
				}
			});

			dispatcher.mouse_up.register_handler([&](MouseUp input) {
				if (input.button == sent_button)
				{
					recv_up++;
				}
			});

			dispatcher.notify_mouse_button(PRESS, sent_button, 0.0f, 0.0f);
			dispatcher.notify_mouse_button(RELEASE, sent_button, 0.0f, 0.0f);
			dispatcher.notify_mouse_button(PRESS, sent_button, 0.0f, 0.0f);
			dispatcher.notify_mouse_button(RELEASE, sent_button, 0.0f, 0.0f);

			test_equal(2, recv_down);
			test_equal(2, recv_up);
		}

		/// \test Tests that InputDispatcher raises the mouse move handler
		/// correctly.
		void test_mouse_move()
		{
			int move_ct = 0;
			float last_x = 0.0f, last_y = 0.0f;

			dispatcher.mouse_move.register_handler([&](MouseMove input) {
				move_ct++;
				last_x = input.cur_x;
				last_y = input.cur_y;
			});

			dispatcher.notify_mouse_move(5.0f, 10.0f);
			dispatcher.notify_mouse_move(30.0f, 50.0f);
			dispatcher.notify_mouse_move(60.0f, -20.0f);

			test_equal(3, move_ct);
			test_equal(60.0f, last_x);
			test_equal(-20.0f, last_y);
		}

		/// \test Tests that InputDispatcher raises the mouse drag handler
		/// when the mouse moves after a left button press and stops sending
		/// after a left button release.
		void test_mouse_drag()
		{
			int drag_ct = 0;
			float dist_x = 0.0f, dist_y = 0.0f;

			dispatcher.mouse_drag.register_handler([&](MouseDrag input) {
				drag_ct++;
				dist_x += input.delta_x;
				dist_y += input.delta_y;
			});

			dispatcher.notify_mouse_button(PRESS, BUTTON_LEFT, 0.0f, 0.0f);
			dispatcher.notify_mouse_move(5.0f, 10.0f);
			dispatcher.notify_mouse_move(30.0f, 50.0f);
			dispatcher.notify_mouse_move(60.0f, -20.0f);
			dispatcher.notify_mouse_button(RELEASE, BUTTON_LEFT, 60.0f, -20.0f);
			dispatcher.notify_mouse_move(80.0f, 0.0f);

			test_equal(3, drag_ct);
			test_equal(60.0f, dist_x);
			test_equal(-20.0f, dist_y);
		}

		/// \test Tests that InputDispatcher raises the mouse scroll handler
		/// correctly.
		void test_mouse_scroll()
		{
			InputDispatcher dispatcher;

			int scroll_ct = 0;
			float dist_x = 0.0f, dist_y = 0.0f;

			dispatcher.mouse_scroll.register_handler([&](MouseScroll input) {
				scroll_ct++;
				dist_x += input.delta_x;
				dist_y += input.delta_y;
			});

			dispatcher.notify_mouse_scroll(5.0f, 10.0f);
			dispatcher.notify_mouse_scroll(25.0f, 40.0f);
			dispatcher.notify_mouse_scroll(30.0f, -70.0f);

			test_equal(3, scroll_ct);
			test_equal(60.0f, dist_x);
			test_equal(-20.0f, dist_y);
		}

		/// \test Tests that InputDispatcher passes the correct mods to a
		/// drag event.
		void test_mouse_drag_mods()
		{
			InputDispatcher dispatcher;

			int none_ct = 0;
			int ctrl_ct = 0;

			dispatcher.mouse_drag.register_handler([&](MouseDrag input) {
				if (input.mods.none())
				{
					none_ct++;
				}
				else if (input.mods[MOD_CTRL])
				{
					ctrl_ct++;
				}
			});

			dispatcher.notify_mouse_button(PRESS, BUTTON_LEFT, 0.0f, 0.0f);
			dispatcher.notify_mouse_move(5.0f, 10.0f);
			dispatcher.notify_mod_input(PRESS, MOD_CTRL);
			dispatcher.notify_mouse_move(30.0f, 50.0f);
			dispatcher.notify_mouse_move(30.0f, 55.0f);
			dispatcher.notify_mod_input(RELEASE, MOD_CTRL);
			dispatcher.notify_mouse_move(60.0f, -20.0f);
			dispatcher.notify_mouse_button(RELEASE, BUTTON_LEFT, 60.0f, -20.0f);

			test_equal(2, none_ct);
			test_equal(2, ctrl_ct);
		}
	};

}   // namespace glge::test::cases

int main()
{
	using glge::test::Test;
	using glge::test::cases::InputTest;

	Test::run(&InputTest::test_single_key);
	Test::run(&InputTest::test_repeat_key);
	Test::run(&InputTest::test_mouse_button);
	Test::run(&InputTest::test_mouse_move);
	Test::run(&InputTest::test_mouse_drag);
	Test::run(&InputTest::test_mouse_drag_mods);
	Test::run(&InputTest::test_mouse_scroll);
}

#include "glge/support/event/input/input_dispatcher.h"

#include <glge/util/util.h>

#include <algorithm>

namespace glge::event::input
{
	void InputDispatcher::update_inputs(LogicalKey key)
	{
		switch (key_states[key])
		{
		case UP:
			active_inputs.erase(key);
			break;
		case DOWN:
			KeyInput new_input{key, mod_states};

			key_press.raise(new_input);

			active_inputs[key] = new_input;

			break;
		}
	}

	InputDispatcher::InputDispatcher() :
		key_states(UP), mod_states(UP), drag_state(NOT_DRAGGING),
		drag_point(0, 0)
	{}

	void InputDispatcher::notify_key_input(LogicalAction action, LogicalKey key)
	{
		if (key == KEY_UNKNOWN)
		{
			return;
		}

		KeyState new_state = DOWN;
		switch (action)
		{
		case PRESS:
			new_state = DOWN;
			break;
		case RELEASE:
			new_state = UP;
			break;
		}

		key_states[key] = new_state;

		update_inputs(key);
	}

	void InputDispatcher::notify_mod_input(LogicalAction action, LogicalMod mod)
	{
		if (mod == NOT_MOD)
		{
			return;
		}

		KeyState new_state = DOWN;
		switch (action)
		{
		case PRESS:
			new_state = DOWN;
			break;
		case RELEASE:
			new_state = UP;
			break;
		}

		mod_states[mod] = new_state;
	}

	void InputDispatcher::notify_mouse_button(LogicalAction action,
											  LogicalButton btn,
											  float cur_x,
											  float cur_y)
	{

		switch (action)
		{
		case PRESS:
			if (btn == BUTTON_LEFT)
			{
				drag_state = DRAGGING;
				drag_point = vec2(cur_x, cur_y);
			}
			mouse_down.raise(MouseDown{btn, mod_states});
			break;
		case RELEASE:
			if (btn == BUTTON_LEFT)
			{
				drag_state = NOT_DRAGGING;
			}
			mouse_up.raise(MouseUp{btn, mod_states});
			break;
		}
	}

	void InputDispatcher::notify_mouse_move(float cur_x, float cur_y)
	{
		switch (drag_state)
		{
		case DRAGGING:
		{
			auto delta_x = (cur_x - drag_point.x);
			auto delta_y = (cur_y - drag_point.y);

			mouse_drag.raise(MouseDrag{delta_x, delta_y, mod_states});

			drag_point = vec2(cur_x, cur_y);
			break;
		}
		case NOT_DRAGGING:
			mouse_move.raise(MouseMove{cur_x, cur_y, mod_states});
			break;
		}
	}

	void InputDispatcher::notify_mouse_scroll(float delta_x, float delta_y)
	{
		mouse_scroll.raise(MouseScroll{delta_x, delta_y, mod_states});
	}

	void InputDispatcher::poll_key_inputs()
	{
		auto raise_events = [&](const std::pair<LogicalKey, KeyInput> & pair) {
			key_press.raise(pair.second);
		};

		std::for_each(active_inputs.cbegin(), active_inputs.cend(), raise_events);
	}
}   // namespace glge::event::input

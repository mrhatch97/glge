#include "glge/support/event/input/input_controller.h"

#include <glge/util/util.h>

#include <algorithm>

namespace glge
{
	namespace input
	{
		const float InputController::rotation_dampening = 3.0f;
		const float InputController::camera_speed = 1.0f;

		void InputController::update_key_state(LogicalKey key, KeyState state)
		{
			key_states[key] = state;
		}

		void InputController::update_mod_state(LogicalMod mod, KeyState state)
		{
			mod_states[mod] = state;
		}

		void InputController::update_inputs(LogicalKey key)
		{
			switch (key_states[key])
			{
			case UP:
				active_inputs.erase(key);
				break;
			case DOWN:
				KeyInput new_input{key, mod_states};

				if (input_binds.find(new_input) == input_binds.cend())
				{
					return;
				}

				auto input_bind = input_binds.at(new_input);

				parent.raise(input_bind.command);

				if (input_bind.should_repeat)
				{
					active_inputs[key] = new_input;
				}

				break;
			}
		}

		InputController::InputController(InputConsumer & parent) :
			parent(parent), key_states(UP), mod_states(UP),
			drag_state(NOT_DRAGGING), drag_point(0, 0)
		{}

		void InputController::key_in(LogicalAction action, LogicalKey key)
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

			update_key_state(key, new_state);
			update_inputs(key);
		}

		void InputController::mod_in(LogicalAction action, LogicalMod mod)
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

			update_mod_state(mod, new_state);
		}

		void InputController::mouse_click(LogicalAction action,
										  LogicalButton btn,
										  float cur_x,
										  float cur_y)
		{
			if (btn != BUTTON_LEFT)
			{
				return;
			}

			switch (action)
			{
			case PRESS:
				drag_state = DRAGGING;
				drag_point = vec2(cur_x, cur_y);
				break;
			case RELEASE:
				drag_state = NOT_DRAGGING;
				break;
			}
		}

		void InputController::mouse_mov(float cur_x, float cur_y)
		{
			switch (drag_state)
			{
			case DRAGGING:
			{
				// auto delta_x = (cur_x - drag_point.x) / rotation_dampening;
				// auto delta_y = -(cur_y - drag_point.y) / rotation_dampening;

				// parent.raise(CommandRollEvent{ INVERT, glm::radians(delta_x)
				// }); parent.raise(CommandPitchEvent{ NORMAL,
				// glm::radians(delta_y) });

				drag_point = vec2(cur_x, cur_y);
			}
			case NOT_DRAGGING:
				return;
			}
		}

		void InputController::mouse_scroll(float, float)
		{
			// parent.raise(CommandSurgeEvent{ NORMAL, delta_y });
		}

		void InputController::poll_key_inputs()
		{
			auto raise_events =
				[&](std::pair<const LogicalKey, KeyInput> & pair) {
					parent.raise(input_binds.at(pair.second).command);
				};

			std::for_each(active_inputs.begin(), active_inputs.end(),
						  raise_events);
		}
	}   // namespace input
}   // namespace glge

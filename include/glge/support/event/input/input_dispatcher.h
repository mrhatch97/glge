#pragma once

#include <glge/common.h>
#include <glge/support/event/event.h>
#include <glge/support/event/input/inputs.h>

#include <unordered_map>

namespace glge::event::input
{
	class InputDispatcher
	{
		enum KeyState : bool
		{
			UP = false,
			DOWN = true
		};

		enum DragState : bool
		{
			DRAGGING = true,
			NOT_DRAGGING = false
		};

		key_bitset key_states;
		mod_bitset mod_states;
		DragState drag_state;
		vec2 drag_point;

		std::unordered_map<LogicalKey, KeyInput> active_inputs;

		void update_inputs(LogicalKey key);

	public:
		Event<KeyInput> key_press;
		Event<MouseDown> mouse_down;
		Event<MouseUp> mouse_up;
		Event<MouseMove> mouse_move;
		Event<MouseDrag> mouse_drag;
		Event<MouseScroll> mouse_scroll;

		InputDispatcher();

		/// <summary>
		/// Call to notify this InputController of a new keyboard input.
		/// </summary>
		/// <param name="action">Action the user took.</param>
		/// <param name="key">Key the user pressed.</param>
		void notify_key_press(LogicalAction action, LogicalKey key);

		/// <summary>
		/// Call to notify this InputController of a new modifier input.
		/// </summary>
		/// <param name="action">Action the user took.</param>
		/// <param name="mod">Modifier the user pressed.</param>
		void notify_mod_press(LogicalAction action, LogicalMod mod);

		/// <summary>
		/// Call to notify this InputController of a mouse click.
		/// </summary>
		/// <param name="action">Action the user took.</param>
		/// <param name="btn">Button the user pressed.</param>
		/// <param name="cur_x">Current x-coordinate of the cursor.</param>
		/// <param name="cur_y">Current y-coordinate of the cursor.</param>
		void notify_mouse_button(LogicalAction action,
								 LogicalButton btn,
								 float cur_x,
								 float cur_y);

		/// <summary>
		/// Call to notify this InputController of a mouse movement.
		/// </summary>
		/// <param name="cur_x">Current x-coordinate of the cursor.</param>
		/// <param name="cur_y">Current y-coordinate of the cursor.</param>
		void notify_mouse_move(float cur_x, float cur_y);

		/// <summary>
		/// Call to notify this InputController of a mouse scroll.
		/// </summary>
		/// <param name="delta_x">
		/// Distance scrolled in the x direction.
		/// </param>
		/// <param name="delta_y">
		/// Distance scrolled in the y direction.
		/// </param>
		void notify_mouse_scroll(float delta_x, float delta_y);

		/// <summary>
		/// Call to raise input events based on the current state of the
		/// InputController.
		/// </summary>
		void poll_key_inputs();
	};
}   // namespace glge::event::input

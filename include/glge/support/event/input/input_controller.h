#pragma once

#include <glge/common.h>

#include <any>
#include <bitset>
#include <functional>
#include <map>
#include <unordered_map>

namespace glge
{
	namespace input
	{
		// TODO switch this to use registration of event handler functors
		// instead of event handler classes

		/// <summary>
		/// Interface for a consumer of input events.
		/// </summary>
		class InputConsumer
		{
		public:
			/// <summary>Notifies the consumer that an event was
			/// raised.</summary>
			virtual void raise(const std::any & e) = 0;
			virtual ~InputConsumer(){};
		};

		/// <summary>
		/// Class that manages user input, input states, and input event
		/// dispatching.
		/// </summary>
		class InputController
		{
		public:
			/// <summary>
			/// Action associated with button input.
			/// </summary>
			enum LogicalAction : bool
			{
				PRESS = true,
				RELEASE = false
			};

			/// <summary>
			/// Key associated with keyboard input.
			/// </summary>
			enum LogicalKey
			{
				KEY_A,
				KEY_B,
				KEY_C,
				KEY_D,
				KEY_E,
				KEY_F,
				KEY_G,
				KEY_H,
				KEY_I,
				KEY_J,
				KEY_K,
				KEY_L,
				KEY_M,
				KEY_N,
				KEY_O,
				KEY_P,
				KEY_Q,
				KEY_R,
				KEY_S,
				KEY_T,
				KEY_U,
				KEY_V,
				KEY_W,
				KEY_X,
				KEY_Y,
				KEY_Z,

				KEY_0,
				KEY_1,
				KEY_2,
				KEY_3,
				KEY_4,
				KEY_5,
				KEY_6,
				KEY_7,
				KEY_8,
				KEY_9,

				KEY_F1,
				KEY_F2,
				KEY_F3,
				KEY_F4,
				KEY_F5,
				KEY_F6,
				KEY_F7,
				KEY_F8,
				KEY_F9,
				KEY_F10,
				KEY_F11,
				KEY_F12,

				KEY_ESCAPE,

				MAX_KEY_DEFINED = KEY_ESCAPE,

				KEY_UNKNOWN
			};

			/// <summary>
			/// Keyboard modifier associated with keyboard
			/// input.
			/// </summary>
			enum LogicalMod
			{
				MOD_SHIFT,
				MOD_CTRL,
				MOD_ALT,

				MAX_MOD_DEFINED = MOD_ALT,

				NOT_MOD = -1
			};

			/// <summary>
			/// Button associated with mouse input.
			/// </summary>
			enum LogicalButton
			{
				BUTTON_LEFT,
				BUTTON_RIGHT,

				MAX_BUTTON_DEFINED = BUTTON_RIGHT,

				BUTTON_UNKNOWN
			};

			/// <summary>
			/// Construct a new InputController with the given InputConsumer
			/// as the consumer for its events.
			/// </summary>
			InputController(InputConsumer & parent);

			/// <summary>
			/// Call to notify this InputController of a new keyboard
			/// input.
			/// </summary>
			/// <param name="action">Action the user took.</param>
			/// <param name="key">Key the user pressed.</param>
			void key_in(LogicalAction action, LogicalKey key);

			/// <summary>
			/// Call to notify this InputController of a new modifier
			/// input.</summary>
			/// </summary>
			/// <param name="action">Action the user took.</param>
			/// <param name="mod">Modifier the user pressed.</param>
			void mod_in(LogicalAction action, LogicalMod mod);

			/// <summary>
			/// Call to notify this InputController of a mouse click.</summary>
			/// </summary>
			/// <param name="action">Action the user took.</param>
			/// <param name="btn">Button the user pressed.</param>
			/// <param name="cur_x">Current x-coordinate of the cursor.</param>
			/// <param name="cur_y">Current y-coordinate of the cursor.</param>
			void mouse_click(LogicalAction action,
							 LogicalButton btn,
							 float cur_x,
							 float cur_y);

			/// <summary>
			/// Call to notify this InputController of a mouse
			/// movement.
			/// </summary>
			/// <param name="cur_x">Current x-coordinate of the cursor.</param>
			/// <param name="cur_y">Current y-coordinate of the cursor.</param>
			void mouse_mov(float cur_x, float cur_y);

			/// <summary>
			/// Call to notify this InputController of a mouse scroll.</summary>
			/// </summary>
			/// <param name="delta_x">
			/// Distance scrolled in the x direction.
			/// </param>
			/// <param name="delta_y">
			/// Distance scrolled in the y direction.
			/// </param>
			void mouse_scroll(float delta_x, float delta_y);

			/// <summary>
			/// Call to raise input events based on the current state of the
			/// InputController.
			/// </summary>
			void poll_key_inputs();

		private:
			static const float rotation_dampening;
			static const float camera_speed;

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

			enum InputRepeat : bool
			{
				REPEAT = true,
				NO_REPEAT = false
			};

			using key_bitset = std::bitset<MAX_KEY_DEFINED + 1>;
			using mod_bitset = std::bitset<MAX_MOD_DEFINED + 1>;

			struct KeyInput
			{
				LogicalKey key;
				mod_bitset mods;

				KeyInput() : key(KEY_UNKNOWN), mods() {}

				KeyInput(LogicalKey key, const mod_bitset & mods) :
					key(key), mods(mods)
				{}

				bool operator<(const KeyInput & other) const
				{
					return key < other.key;
				}

				bool operator>(const KeyInput & other) const
				{
					return key > other.key;
				}

				bool operator==(const KeyInput & other) const
				{
					return key == other.key;
				}
			};

			struct KeyCommand
			{
				std::any command;
				InputRepeat should_repeat;

				KeyCommand() : command(), should_repeat(NO_REPEAT) {}

				KeyCommand(const std::any & command,
						   InputRepeat should_repeat) :
					command(command),
					should_repeat(should_repeat)
				{}
			};

			InputConsumer & parent;

			key_bitset key_states;
			mod_bitset mod_states;
			DragState drag_state;
			vec2 drag_point;

			std::map<KeyInput, KeyCommand> input_binds;
			std::unordered_map<LogicalKey, KeyInput> active_inputs;

			void update_key_state(LogicalKey key, KeyState state);
			void update_mod_state(LogicalMod mod, KeyState state);
			void update_inputs(LogicalKey key);
		};
	}   // namespace input
}   // namespace glge

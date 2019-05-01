#pragma once

#include <glge/common.h>

#include <bitset>

namespace glge::event::input
{
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

  using key_bitset = std::bitset<MAX_KEY_DEFINED + 1>;

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

  struct MouseDown
  {
    LogicalButton button;
    mod_bitset mods;
  };

  struct MouseUp
  {
    LogicalButton button;
    mod_bitset mods;
  };

  struct MouseMove
  {
    float cur_x, cur_y;
    mod_bitset mods;
  };

  struct MouseDrag
  {
    float delta_x, delta_y;
    mod_bitset mods;
  };

  struct MouseScroll
  {
    float delta_x, delta_y;
    mod_bitset mods;
  };
}

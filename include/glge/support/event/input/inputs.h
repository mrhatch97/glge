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

  /// <summary>
  /// Type alias for a std::bitset containg key press states.
  /// </summary>
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

  /// <summary>
  /// Type alias for a std::bitset containg mod states.
  /// </summary>
  using mod_bitset = std::bitset<MAX_MOD_DEFINED + 1>;

  /// <summary>
  /// Representation of a keyboard input (key + mods), e.g. CTRL + A
  /// </summary>
  struct KeyInput
  {
    /// <summary>Main key for the input.</summary>
    LogicalKey key;
    /// <summary>Mods active when this input was made.</summary>
    mod_bitset mods;

    /// <summary>
    /// Default-construct a KeyInput. Provided for use with std::map only.
    /// </summary>
    KeyInput() : key(KEY_UNKNOWN), mods() {}

    /// <summary>
    /// Construct a KeyInput representing the given key and mod combination.
    /// </summary>
    /// <param name="key">
    /// Main key for the input.
    /// </param>
    /// <param name="mods">
    /// Mod states at the time of the input.
    /// </param>
    KeyInput(LogicalKey key, const mod_bitset & mods) :
      key(key), mods(mods)
    {}

    /// <summary>
    /// Less-than compare this KeyInput's key value to another KeyInput.
    /// </summary>
    /// <param name="other">
    /// KeyInput to compare to.
    /// </param>
    /// <returns>True if this key is less than other.key.</returns>
    bool operator<(const KeyInput & other) const
    {
      return key < other.key;
    }

    /// <summary>
    /// Greater-than this KeyInput's key value to another KeyInput.
    /// </summary>
    /// <param name="other">
    /// KeyInput to compare to.
    /// </param>
    /// <returns>True if this key is greater than other.key.</returns>
    bool operator>(const KeyInput & other) const
    {
      return key > other.key;
    }

    /// <summary>
    /// Equality-compare this KeyInput's key value to another KeyInput.
    /// </summary>
    /// <param name="other">
    /// KeyInput to compare to.
    /// </param>
    /// <returns>True if this key is equal to other.key.</returns>
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

  /// <summary>
  /// Message object for a mouse button down event.
  /// </summary>
  struct MouseDown
  {
    /// <summary>
    /// Button that was pressed down.
    /// </summary>
    LogicalButton button;

    /// <summary>
    /// Mod states at the time of the event.
    /// </summary>
    mod_bitset mods;
  };

  /// <summary>
  /// Message object for a mouse button up event.
  /// </summary>
  struct MouseUp
  {
    /// <summary>
    /// Button that was released.
    /// </summary>
    LogicalButton button;

    /// <summary>
    /// Mod states at the time of the event.
    /// </summary>
    mod_bitset mods;
  };

  /// <summary>
  /// Message object for a mouse move event.
  /// </summary>
  struct MouseMove
  {
    /// <summary>
    /// The x-position the mouse moved to.
    /// </summary>
    float cur_x;

    /// <summary>
    /// The y-position the mouse moved to.
    /// </summary>
    float cur_y;

    /// <summary>
    /// Mod states at the time of the event.
    /// </summary>
    mod_bitset mods;
  };

  /// <summary>
  /// Message object for a mouse drag event.
  /// </summary>
  struct MouseDrag
  {
    /// <summary>
    /// The x-displacement the mouse moved by.
    /// </summary>
    float delta_x;

    /// <summary>
    /// The y-displacement the mouse moved by.
    /// </summary>
    float delta_y;

    /// <summary>
    /// Mod states at the time of the event.
    /// </summary>
    mod_bitset mods;
  };

  /// <summary>
  /// Message object for a mouse scroll event.
  /// </summary>
  struct MouseScroll
  {
    /// <summary>
    /// The x-displacement (horizontal) the scroll wheel moved by.
    /// </summary>
    float delta_x;

    /// <summary>
    /// The y-displacement (vertical) the scroll wheel moved by.
    /// </summary>
    float delta_y;

    /// <summary>
    /// Mod states at the time of the event.
    /// </summary>
    mod_bitset mods;
  };
}

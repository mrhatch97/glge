#pragma once

#include <variant>

namespace eng
{
	namespace input
	{
		namespace events
		{
			enum DirectionInvert : bool
			{
				NORMAL = false,
				INVERT = true
			};

			// 6DOF movements - https://en.wikipedia.org/wiki/Six_degrees_of_freedom

			// Translational envelopes

			// Forward/back
			struct CommandSurgeEvent
			{
				bool invert;
				float dist;
			};

			// Left/right
			struct CommandSwayEvent
			{
				bool invert;
				float dist;
			};

			// Up/down
			struct CommandHeaveEvent
			{
				bool invert;
				float dist;
			};

			// Rotational envelopes

			struct CommandRollEvent
			{
				bool invert;
				float angle;
			};

			struct CommandPitchEvent
			{
				bool invert;
				float angle;
			};

			struct CommandYawEvent
			{
				bool invert;
				float angle;
			};
			
			// Window commands

			struct CommandToggleVFCullingEvent
			{
			};

			struct CommandToggleBoundingSpheresEvent
			{
			};

			struct CommandCloseWindowEvent
			{
			};

			struct CommandSetSceneEvent
			{
				unsigned int scene;
			};

			struct CommandTogglePauseEvent
			{
			};

			using InputEvent = std::variant<
				CommandSurgeEvent,
				CommandSwayEvent,
				CommandHeaveEvent,
				CommandRollEvent,
				CommandPitchEvent,
				CommandYawEvent,
				CommandToggleVFCullingEvent,
				CommandToggleBoundingSpheresEvent,
				CommandCloseWindowEvent,
				CommandSetSceneEvent,
				CommandTogglePauseEvent
			>;
		}
	}
}
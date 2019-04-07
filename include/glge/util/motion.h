#pragma once

#include <glge/common.h>

namespace glge::util
{
	struct Placement
	{
		mat4 transform;

		vec3 get_right_direction() const
		{
			return transform[0];
		}

		vec3 get_up_direction() const
		{
			return transform[1];
		}

		vec3 get_forward_direction() const
		{
			return transform[2];
		}

		vec3 get_position() const
		{
			return transform[3];
		}
	};
}
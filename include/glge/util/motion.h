#pragma once

#include <glge/common.h>

namespace glge::util
{
	class Placement
	{
	private:
		vec3 position;
		vec3 director;
		vec3 up_dir;
		mat4 transform;

	public:
		Placement();
		Placement(vec3 initial_pos, vec3 initial_forward, vec3 initial_up);

		void forward(float dist);
		void back(float dist);
		void up(float dist);
		void down(float dist);
		void left(float dist);
		void right(float dist);

		void yaw_left(float angle);
		void yaw_right(float angle);
		void pitch_up(float angle);
		void pitch_down(float angle);
		void roll_left(float angle);
		void roll_right(float angle);

		vec3 get_position() const
		{
			return position;
		}

		vec3 get_up_direction() const
		{
			return up_dir;
		}

		vec3 get_forward_direction() const
		{
			return director;
		}

		mat4 get_transform() const
		{
			return transform;
		}

		vec3 get_right_direction() const;
	};
}
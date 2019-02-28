#pragma once

#include <glge/Util/Util.h>
#include <glge/Common.h>

namespace eng
{
	namespace lsystem
	{
		class LTurtle {
		private:
			vec3 position;
			mat4 orientation_mat;

		public:
			LTurtle();
			LTurtle(vec3 pos, int head);
			vec3 getPosition();
			mat4 getOrientation();
			 

			void setPosition(vec3 newPos);
			void setOrientation(mat4 newOrientation);

			void roll(int degrees);
			void pitch(int degrees);
			void turn(int degrees);
			void moveForward(float units);

		};
	}
}
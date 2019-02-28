#pragma once

#include <glge/Common.h>
#include <glge/Util/Util.h>

#include "LTurtle.h"

#include <forward_list>
#include <stack>
#include <tuple>

namespace eng
{
	namespace lsystem
	{
		class Tree {
		public:
			Tree();
			//  model transform, colorLeaf bool
			std::forward_list<std::tuple<mat4, bool>> transforms;
			void generate(float length, char rule, int curIter, int numIter, bool colorLeaf);


		private:
			LTurtle turtle;
			std::stack<vec3> loc_stack;
			std::stack<mat4> orientation_stack;
		};
	}
}
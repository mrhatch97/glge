#pragma once

#include <glge/common.h>
#include <glge/util/util.h>

#include "l_turtle.h"

#include <forward_list>
#include <stack>
#include <tuple>

namespace glge::proc_gen
{
	class Tree
	{
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

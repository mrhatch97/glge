#pragma once

#include "glge/renderer/scene_graph/scene.h"
#include "base_dispatcher.h"

#include <glge/common.h>

#include <forward_list>

namespace glge::renderer::scene_graph
{
	struct Node
	{
		std::forward_list<unique_ptr<Node>> children;

		virtual ~Node() = default;

		virtual mat4 accept(const BaseDispatcher & dispatcher, mat4 cur_M) const
		{
			return dispatcher.dispatch(*this, cur_M);
		}
	};
}

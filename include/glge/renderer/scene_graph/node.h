#pragma once

#include <glge/common.h>
#include <glge/util/motion.h>
#include <glge/util/math.h>
#include <glge/renderer/primitives/shader_program.h>
#include <glge/renderer/scene_graph/scene_settings.h>

#include <forward_list>

namespace glge::renderer::scene_graph
{
	class BaseSceneDispatcher;

	struct Node
	{
	public:
		Node();

		std::forward_list<uptr<Node>> children;

		virtual ~Node();

		virtual void accept(const BaseSceneDispatcher & dispatcher) const = 0;
	};
}

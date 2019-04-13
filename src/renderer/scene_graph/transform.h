#pragma once

#include "node.h"

namespace glge::renderer::scene_graph
{
	struct SceneTransform : public Node
	{
		const util::Placement & placement;

		SceneTransform(const util::Placement & placement) : placement(placement)
		{}

		mat4 accept(const BaseDispatcher & dispatcher,
					mat4 cur_M) const override
		{
			return dispatcher.dispatch(*this, cur_M);
		}
	};
}   // namespace glge::renderer::scene_graph

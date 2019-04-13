#pragma once

#include <glge/common.h>

namespace glge::renderer::scene_graph
{
	struct Node;
	struct Geometry;
	struct SceneTransform;
	struct SceneCamera;

	// TODO use CRTP to optimize choice of dispatcher?
	class BaseDispatcher
	{
	public:
		virtual mat4 dispatch(const Node & node, mat4 cur_M) const = 0;
		virtual mat4 dispatch(const Geometry & node, mat4 cur_M) const = 0;
		virtual mat4 dispatch(const SceneTransform & node,
							  mat4 cur_M) const = 0;
		virtual mat4 dispatch(const SceneCamera & node, mat4 cur_M) const = 0;

		virtual ~BaseDispatcher() = default;
	};
}   // namespace glge::renderer::scene_graph
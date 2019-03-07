#pragma once

#include <glge/renderer/scene_graph/node.h>
#include <glge/renderer/primitives/renderable.h>

namespace glge::renderer::scene_graph
{
	struct Geometry : public Node
	{
		const primitive::Renderable & renderable;
		const primitive::ShaderInstance & shader;

		Geometry(const primitive::Renderable & renderable, const primitive::ShaderInstance & shader) :
			renderable(renderable), shader(shader)
		{
		}

		virtual void accept(const BaseSceneDispatcher & dispatcher) const
		{
			dispatcher.dispatch(*this);
		}
	};
}

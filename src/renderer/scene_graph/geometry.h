#pragma once

#include "node.h"
#include <glge/renderer/primitives/renderable.h>

namespace glge::renderer::scene_graph
{
	struct Geometry : public Node
	{
		const primitive::Renderable & renderable;
		const primitive::ShaderInstanceBase & shader;

    Geometry(
        primitive::Renderable & renderable, 
        primitive::ShaderInstanceBase & shader
    ) :
      renderable(renderable),
      shader(shader)
    { }

		virtual mat4 accept(const BaseDispatcher & dispatcher, mat4 cur_M) const
		{
			return dispatcher.dispatch(*this, cur_M);
		}
	};
}

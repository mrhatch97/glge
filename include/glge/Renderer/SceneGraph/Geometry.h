#pragma once

#include <glge/Renderer/SceneGraph/Node.h>
#include <glge/Renderer/Primitives/Renderable.h>

namespace eng::render::scene
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
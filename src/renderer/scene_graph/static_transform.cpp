#include "glge/renderer/scene_graph/transform.h"

#include <glge/renderer/scene_graph/traversal.h>

namespace glge::renderer::scene_graph
{
	StaticTransform::StaticTransform(const mat4 & transform) : transform(transform)
	{
	}

	StaticTransform::StaticTransform(const quat & transform) :
		StaticTransform(glm::toMat4(transform))
	{
	}

	StaticTransform::StaticTransform(const StaticTransform & other)
		: StaticTransform(other.transform)
	{
	}

	void StaticTransform::accept(const BaseSceneDispatcher & dispatcher) const
	{
		dispatcher.dispatch(*this);
	}
}

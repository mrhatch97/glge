#include "glge/Renderer/SceneGraph/Transform.h"

#include <glge/Renderer/SceneGraph/Traversal.h>

namespace eng::render::scene
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
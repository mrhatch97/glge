#include "glge/Renderer/SceneGraph/Transform.h"

#include <glge/Renderer/SceneGraph/Traversal.h>

namespace eng::render::scene
{
	DynamicTransform::DynamicTransform(const util::Placement & placement) :
		placement(placement)
	{ 
	}

	void DynamicTransform::accept(const BaseSceneDispatcher & dispatcher) const
	{
		dispatcher.dispatch(*this);
	}
}
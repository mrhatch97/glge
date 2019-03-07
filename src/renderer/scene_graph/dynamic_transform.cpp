#include "glge/renderer/scene_graph/transform.h"

#include <glge/renderer/scene_graph/traversal.h>

namespace glge::renderer::scene_graph
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

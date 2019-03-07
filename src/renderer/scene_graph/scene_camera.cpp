#include "glge/renderer/scene_graph/scene_camera.h"

#include <glge/renderer/scene_graph/traversal.h>

namespace glge::renderer::scene_graph
{
	SceneCamera::SceneCamera() = default;

	void SceneCamera::accept(const BaseSceneDispatcher& dispatcher) const
	{
		dispatcher.dispatch(*this);
	}
}

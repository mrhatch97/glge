#include "glge/Renderer/SceneGraph/SceneCamera.h"

#include <glge/Renderer/SceneGraph/Traversal.h>

namespace eng::render::scene
{
	SceneCamera::SceneCamera() = default;

	void SceneCamera::accept(const BaseSceneDispatcher& dispatcher) const
	{
		dispatcher.dispatch(*this);
	}
}
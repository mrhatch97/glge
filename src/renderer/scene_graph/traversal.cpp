#include "glge/renderer/scene_graph/traversal.h"

#include <glge/renderer/scene_graph/geometry.h>
#include <glge/renderer/scene_graph/transform.h>
#include <glge/renderer/scene_graph/scene_camera.h>

namespace glge::renderer::scene_graph
{
	BaseSceneDispatcher::BaseSceneDispatcher(const SceneSettings & settings) :
		settings(settings)
	{ }

	RenderingSceneDispatcher::RenderingSceneDispatcher(const SceneSettings & settings, Renderer & renderer) :
		BaseSceneDispatcher(settings), renderer(renderer), cur_M(1)
	{ }

	void RenderingSceneDispatcher::dispatch(const Geometry & node) const
	{
		renderer.enqueue(node.renderable, node.shader, cur_M);
	}

	void RenderingSceneDispatcher::dispatch(const DynamicTransform & node) const
	{
		return;
	}

	void RenderingSceneDispatcher::dispatch(const StaticTransform & node) const
	{
		return;
	}

	void RenderingSceneDispatcher::dispatch(const SceneCamera & node) const
	{
		return;
	}
}

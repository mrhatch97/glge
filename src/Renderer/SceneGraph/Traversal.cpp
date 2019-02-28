#include "glge/Renderer/SceneGraph/Traversal.h"

#include <glge/Renderer/SceneGraph/Geometry.h>
#include <glge/Renderer/SceneGraph/Transform.h>
#include <glge/Renderer/SceneGraph/SceneCamera.h>

namespace eng::render::scene
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
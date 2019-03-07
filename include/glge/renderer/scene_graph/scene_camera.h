#pragma once

#include <glge/renderer/scene_graph/node.h>
#include <glge/renderer/camera.h>

namespace glge::renderer::scene_graph
{
	struct SceneCamera
	{
		util::CameraIntrinsics camera_intrinsics;

		SceneCamera();

		virtual void accept(const BaseSceneDispatcher& dispatcher) const;
	};
}

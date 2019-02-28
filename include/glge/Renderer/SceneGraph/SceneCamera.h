#pragma once

#include <glge/Renderer/SceneGraph/Node.h>
#include <glge/Renderer/Camera.h>

namespace eng::render::scene
{
	struct SceneCamera
	{
		util::CameraIntrinsics camera_intrinsics;

		SceneCamera();

		virtual void accept(const BaseSceneDispatcher& dispatcher) const;
	};
}
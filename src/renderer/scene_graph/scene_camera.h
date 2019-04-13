#pragma once

#include "node.h"
#include <glge/renderer/camera.h>

namespace glge::renderer::scene_graph
{
	struct SceneCamera : public Node
	{
		CameraIntrinsics camera_intrinsics;
		bool active = false;

		SceneCamera(const CameraIntrinsics & intrinsics) :
			camera_intrinsics(intrinsics)
		{}

		mat4 accept(const BaseDispatcher & dispatcher,
					mat4 cur_M) const override
		{
			return dispatcher.dispatch(*this, cur_M);
		}

		unique_ptr<Camera> get_camera(mat4 M) const
		{
			return std::make_unique<Camera>(camera_intrinsics,
											util::Placement{M});
		}
	};
}   // namespace glge::renderer::scene_graph

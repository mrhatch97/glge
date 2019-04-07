#pragma once

#include <glge/common.h>

namespace glge::renderer::scene_graph
{
	struct SceneCamera;

	struct SceneSettings
	{
		observer_ptr<const SceneCamera> active_camera;

		bool draw_bounding_spheres;
		bool enable_VF_culling;

		SceneSettings(observer_ptr<const SceneCamera> active_camera,
			bool draw_bounding_spheres,
			bool enable_VF_culling);
	};
}

#pragma once

#include <glge/common.h>

namespace glge::renderer::scene_graph
{
	struct SceneCamera;

	struct SceneSettings
	{
		const SceneCamera * active_camera;

		bool draw_bounding_spheres;
		bool enable_VF_culling;

		SceneSettings(const SceneCamera * active_camera,
			bool draw_bounding_spheres,
			bool enable_VF_culling);
	};
}
#pragma once

#include <glge/Common.h>

namespace eng::render::scene
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
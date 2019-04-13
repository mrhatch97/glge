/// <summary>Data objects for scene configuration.</summary>
///
/// Contains data objects for specifying the configuration settings
/// of a Scene.
///
/// \file scene_settings.h

#pragma once

#include <glge/common.h>

namespace glge::renderer::scene_graph
{
	struct SceneCamera;

	/// <summary>
	/// Configuration object for a Scene.
	/// </summary>
	struct SceneSettings
	{
		/// <summary>Pointer to the active camera in the scene.</summary>
		observer_ptr<const SceneCamera> active_camera;

		/// <summary>
		/// Flag controlling whether bounding spheres should be drawn.
		/// </summary>
		bool draw_bounding_spheres;

		/// <summary>
		/// Flag controlling whether view frustum culling should be used.
		/// </summary>
		bool enable_VF_culling;

		/// <summary>
		/// Constructs a new SceneSettings with the given settings.
		/// </summary>
		SceneSettings(observer_ptr<const SceneCamera> active_camera,
					  bool draw_bounding_spheres,
					  bool enable_VF_culling);
	};
}   // namespace glge::renderer::scene_graph

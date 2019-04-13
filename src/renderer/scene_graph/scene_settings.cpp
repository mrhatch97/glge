#include "glge/renderer/scene_graph/scene_settings.h"

namespace glge::renderer::scene_graph
{
	SceneSettings::SceneSettings(const SceneCamera * active_camera,
								 bool draw_bounding_spheres,
								 bool enable_VF_culling) :
		active_camera(active_camera),
		draw_bounding_spheres(draw_bounding_spheres),
		enable_VF_culling(enable_VF_culling)
	{}
}   // namespace glge::renderer::scene_graph

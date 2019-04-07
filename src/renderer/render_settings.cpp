#include "glge/renderer/render_settings.h"

namespace glge::renderer
{
	RenderParameters::RenderParameters(const RenderSettings& settings, mat4 M) :
		settings(settings), M(M), MVP(settings.camera->intrinsics.get_P() * settings.camera->get_V() * M)
	{ }
}

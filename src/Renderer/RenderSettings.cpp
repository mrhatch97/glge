#include "glge/Renderer/RenderSettings.h"

namespace eng::render
{
	RenderSettings::RenderSettings(mat4 V, mat4 P, vec3 cam_pos, vec3 cam_dir) :
		V(V), P(P), cam_pos(cam_pos), cam_dir(cam_dir)
	{ }

	RenderParameters::RenderParameters(const RenderSettings& settings, mat4 M) :
		settings(settings), M(M), MVP(settings.P * settings.V * M)
	{ }
}
#pragma once

#include <glge/common.h>

namespace glge::renderer
{
	struct RenderSettings
	{
		const mat4 V;
		const mat4 P;
		const vec3 cam_pos;
		const vec3 cam_dir;

		RenderSettings(mat4 V, mat4 P, vec3 cam_pos, vec3 cam_dir);
	};

	struct RenderParameters
	{
		const RenderSettings & settings;
		const mat4 M;
		const mat4 MVP;

		RenderParameters(const RenderSettings & settings, mat4 M);
	};
}
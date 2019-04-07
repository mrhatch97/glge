#pragma once

#include <glge/renderer/camera.h>
#include <glge/common.h>

namespace glge::renderer
{
	struct RenderSettings
	{
		unique_ptr<Camera> camera = nullptr;
	};

	struct RenderParameters
	{
		const RenderSettings & settings;
		const mat4 M;
		const mat4 MVP;

		RenderParameters(const RenderSettings & settings, mat4 M);
	};
}
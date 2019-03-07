#pragma once

#include <glge/common.h>
#include <glge/renderer/primitives/renderable.h>
#include <glge/renderer/primitives/shader_program.h>
#include <glge/renderer/render_settings.h>

#include <queue>
#include <variant>

namespace glge::renderer
{
	//template<typename T>
	//constexpr T frame_time_ratio = static_cast<T>(frame_clock::period::num) / static_cast<T>(frame_clock::period::den);

	struct RenderTarget
	{
		const primitive::Renderable & target;
		const primitive::ShaderInstance & shader;
		mat4 M;
	};

	class Renderer
	{
		// TODO use multimap instead of queue to throw into buckets based on underlying shader
		std::queue<RenderTarget> render_queue;

		RenderSettings settings;
	public:
		Renderer(const RenderSettings & settings);

		void enqueue(const primitive::Renderable & target, const primitive::ShaderInstance & shader, mat4 M);

		void render();
	};
}

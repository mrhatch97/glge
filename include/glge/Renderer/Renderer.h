#pragma once

#include <glge/Common.h>
#include <glge/Renderer/Primitives/Renderable.h>
#include <glge/Renderer/Primitives/ShaderProgram.h>
#include <glge/Renderer/RenderSettings.h>

#include <queue>
#include <variant>

namespace eng::render
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
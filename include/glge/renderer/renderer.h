#pragma once

#include <glge/common.h>
#include <glge/renderer/render_settings.h>

#include <map>
#include <typeindex>

namespace glge::renderer
{
	namespace primitive
	{
		class Renderable;
		struct ShaderInstanceBase;
	}

	// TODO find a better place/name for this func?
	void configure_environment();

	struct RenderTarget
	{
		const primitive::Renderable & renderable;
		const primitive::ShaderInstanceBase & shader_instance;
		mat4 M;
	};

	class Renderer
	{
		std::multimap<std::type_index, RenderTarget> render_targets;

	public:
		RenderSettings settings;

		Renderer();

		void enqueue(
			const primitive::Renderable & target, 
			const primitive::ShaderInstanceBase & shader_instance,
			mat4 M
		);

		void render();

		size_t target_count() const;
	};
}

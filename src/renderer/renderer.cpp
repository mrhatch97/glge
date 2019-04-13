#include "glge/renderer/renderer.h"

#include <glge/renderer/primitives/renderable.h>
#include <glge/renderer/primitives/shader_program.h>
#include <glge/renderer/render_settings.h>

namespace glge::renderer
{
	Renderer::Renderer() = default;

	void
	Renderer::enqueue(const primitive::Renderable & target,
					  const primitive::ShaderInstanceBase & shader_instance,
					  mat4 M)
	{
		render_targets.emplace(typeid(shader_instance.shader),
							   RenderTarget{target, shader_instance, M});
	}

	void Renderer::render()
	{
		for (auto iter = render_targets.cbegin(), end = render_targets.cend();
			 iter != end;)
		{
			auto region_start_iter = iter;
			auto shader_bind = iter->second.shader_instance.shader.bind();

			for (; (iter != end) && (iter->first == region_start_iter->first);
				 iter++)
			{
				const RenderTarget & current_target = iter->second;

				RenderParameters params(settings, current_target.M);

				current_target.shader_instance(params);

				current_target.renderable.render();
			}
		}
	}

	size_t Renderer::target_count() const { return render_targets.size(); }
}   // namespace glge::renderer

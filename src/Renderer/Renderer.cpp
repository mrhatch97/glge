#include "glge/Renderer/Renderer.h"

namespace eng::render
{
	void Renderer::enqueue(const primitive::Renderable & target, const primitive::ShaderInstance & shader, mat4 M)
	{
		render_queue.push(RenderTarget{ target, shader, M });
	}

	void Renderer::render()
	{
		for (; !render_queue.empty(); render_queue.pop())
		{
			auto current = render_queue.front();

			RenderParameters params(settings, current.M);

			{
				auto shader_bind = current.shader(params);

				current.target.render();
			}
		}
	}
}
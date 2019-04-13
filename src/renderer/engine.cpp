#include "glge/renderer/engine.h"

namespace glge::renderer
{
	Engine::Engine() : shaders(), continue_running(false) {}

	auto Engine::get_shader_manager() -> primitive::ShaderManager &
	{
		return shaders;
	}

	void Engine::run()
	{
		continue_running = true;
		while (continue_running)
		{
		}
	}
}   // namespace glge::renderer

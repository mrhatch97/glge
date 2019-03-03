#include "glge/Renderer/Engine.h"

namespace eng::render
{
	Engine::Engine() : shaders(), continue_running(false)
	{
	}

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
}

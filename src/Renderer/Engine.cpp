#include "glge/Renderer/Engine.h"

namespace eng::render
{
	Engine::Engine() : resources(), shaders(), continue_running(false)
	{
	}

	res::ResourceManager & Engine::get_resource_manager()
	{
		return resources;
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
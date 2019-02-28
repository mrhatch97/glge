#pragma once

#include <glge/Renderer/Resources/ResourceManager.h>
#include <glge/Renderer/Primitives/ShaderProgram.h>

#include <typeindex>

namespace eng::render
{
	class Engine
	{
	private:
		res::ResourceManager resources;
		primitive::ShaderManager shaders;

		bool continue_running;
	public:
		Engine();

		/// <summary>
		/// Gets a reference to the engine's resource manager.
		/// </summary>
		/// <returns>Reference to the engine's resource manager.</returns>
		res::ResourceManager & get_resource_manager();


		/// <summary>
		/// Gets a reference to the engine's shader manager.
		/// </summary>
		/// <returns>Reference to the engine's shader manager.</returns>
		primitive::ShaderManager & get_shader_manager();

		void run();
	};
}
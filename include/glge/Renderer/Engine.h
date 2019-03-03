#pragma once

#include <glge/Renderer/Primitives/ShaderProgram.h>

#include <typeindex>

namespace eng::render
{
	class Engine
	{
	private:
		primitive::ShaderManager shaders;

		bool continue_running;
	public:
		Engine();

		/// <summary>
		/// Gets a reference to the engine's shader manager.
		/// </summary>
		/// <returns>Reference to the engine's shader manager.</returns>
		primitive::ShaderManager & get_shader_manager();

		void run();
	};
}

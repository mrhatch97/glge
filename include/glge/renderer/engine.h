/// <summary>Contains class for running the engine.</summary>
///
/// \file engine.h

#pragma once

#include <glge/renderer/primitives/shader_program.h>

#include <typeindex>

namespace glge::renderer
{
	/// <summary>
	/// Rendering engine.
	/// </summary>
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

		/// <summary>
		/// Begin the main engine loop.
		/// </summary>
		void run();
	};
}   // namespace glge::renderer

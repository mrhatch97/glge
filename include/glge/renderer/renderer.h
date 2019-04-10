/// <summary>
/// Classes and functions for configuring and running the
/// engine's renderer.
/// </summary>
///
/// \file renderer.h
//
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
  /// <summary>
  /// Configure the rendering backend. Must be called before using any functions
  /// or classes in glge::rendering or errors may occur.
  /// </summary>
	void configure_environment();

  /// <summary>
  /// A target to be rendered along with its shader and Model matrix.
  /// </summary>
	struct RenderTarget
	{
    /// <summary>
    /// Target to be rendered.
    /// </summary>
		const primitive::Renderable & renderable;

    /// <summary>
    /// Shader to use when rendering the target.
    /// </summary>
		const primitive::ShaderInstanceBase & shader_instance;

    /// <summary>
    /// Model matrix of the target.
    /// </summary>
		mat4 M;
	};

  /// <summary>
  /// Container and runner for rendering jobs.
  /// </summary>
  /// Holds a collection of rendering tasks. When all tasks
  /// have been created, runs each task, drawing it to the
  /// current rendering context.
	class Renderer
	{
		std::multimap<std::type_index, RenderTarget> render_targets;

	public:
    /// <summary>
    /// Configuration specifying options for how the renderer should
    /// run.
    /// </summary>
		RenderSettings settings;

    /// <summary>
    /// Constructs a new Renderer with no tasks.
    /// </summary>
		Renderer();

    /// <summary>
    /// Add a new render task with the given target, shader, and Model matrix.
    /// </summary>
		void enqueue(
			const primitive::Renderable & target, 
			const primitive::ShaderInstanceBase & shader_instance,
			mat4 M
		);

    /// <summary>
    /// Run all render tasks.
    /// </summary>
		void render();

    /// <summary>
    /// Get the number of tasks enqueued in this Renderer.
    /// </summary>
    /// <returns>Number of rendering tasks in this Renderer.</returns>
		size_t target_count() const;
	};
}

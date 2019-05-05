/// <summary>Data objects for renderer configuration.</summary>
///
/// Contains data objects for specifying the configuration settings
/// of a Renderer and parameters for rendering a target.
///
/// \file render_settings.h

#pragma once

#include <glge/common.h>
#include <glge/renderer/camera.h>

namespace glge::renderer
{
	/// <summary>
	/// Configuration object for a Renderer.
	/// </summary>
	struct RenderSettings
	{
		/// <summary>
		/// Pointer to a Camera to use to render.
		/// </summary>
		unique_ptr<Camera> camera = nullptr;
	};

	/// <summary>
	/// Contains all information necessary to render a RenderTarget.
	/// </summary>
	struct RenderParameters
	{
		/// <summary>
		/// Reference to settings used to configure renderer.
		/// </summary>
		const RenderSettings & settings;

		/// <summary>
		/// Model matrix of the RenderTarget.
		/// </summary>
		const mat4 M;

		/// <summary>
		/// Precomputed product of the Model, View, and Projection matrices
		/// for the RenderTarget.
		/// </summary>
		const mat4 MVP;

		/// <summary>
		/// Construct a new parameter set with the given settings and Model
		/// matrix.
		/// </summary>
        /// <param name="settings">
        /// RenderSettings to use to render with.
        /// </param>
        /// <param name="M">
        /// Model matrix to render with.
        /// </param>
		RenderParameters(const RenderSettings & settings, mat4 M);
	};
}   // namespace glge::renderer

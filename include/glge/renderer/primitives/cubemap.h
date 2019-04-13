/// <summary>Support for cubemap textures.</summary>
///
/// Contains class for using cubemap textures.
///
/// \file cubemap.h

#pragma once

#include <glge/common.h>
#include <glge/renderer/primitives/primitive_data.h>

namespace glge::renderer::primitive
{
	/// <summary>
	/// Class representing a cubemap texture.
	/// </summary>
	class Cubemap
	{
	public:
		Cubemap() = default;

		virtual ~Cubemap() = default;

		/// <summary>
		/// Set this cubemap as the active texture.
		/// </summary>
		virtual void activate() const = 0;

		/// <summary>
		/// Load a cubemap from files on disk.
		/// </summary>
		/// <param name="file_info">Descriptor for the cubemap files.</param>
		/// <returns>Pointer to created cubemap.</returns>
		static unique_ptr<Cubemap> from_file(const CubemapFileInfo & file_info);
	};
}   // namespace glge::renderer::primitive

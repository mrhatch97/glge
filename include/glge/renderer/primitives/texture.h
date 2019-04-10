/// <summary>
/// Support for 2D textures.
/// </summary>
///
/// Contains class for using 2D textures.
///
/// \file texture.h

#pragma once

#include <glge/common.h>
#include <glge/renderer/primitives/primitive_data.h>

namespace glge::renderer::primitive
{
  /// <summary>
  /// Class representing a 2D texture.
  /// </summary>
	class Texture
	{
	public:
		Texture() = default;

		virtual ~Texture() = default;

    /// <summary>
    /// Set this texture as the texture to be sampled from.
    /// </summary>
		virtual void activate() const = 0;

    /// <summary>
    /// Load a texture from a file on disk.
    /// </summary>
    /// <param name="file_info">Descriptor for the texture file.</param>
    /// <returns>Pointer to created texture.</returns>
		static unique_ptr<Texture> from_file(const TextureFileInfo & file_info);
	};
}

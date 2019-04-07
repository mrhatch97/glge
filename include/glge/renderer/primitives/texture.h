#pragma once

#include <glge/common.h>
#include <glge/renderer/primitives/primitive_data.h>

namespace glge::renderer::primitive
{
	class Texture
	{
	public:
		Texture() = default;

		virtual ~Texture() = default;

		virtual void activate() const = 0;

		static unique_ptr<Texture> from_file(const TextureFileInfo & file_info);
	};
}

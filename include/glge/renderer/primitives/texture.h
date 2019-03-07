#pragma once

#include <glge/common.h>
#include <glge/renderer/primitives/primitive_data.h>

namespace glge::renderer::primitive
{
	namespace opengl
	{
		class GLTexture;
	}

	using TextureImpl = opengl::GLTexture;


	class Texture
	{
	private:
		uptr<TextureImpl> texture;
	public:
		Texture(const TextureData & texture_data);

		~Texture();

		void activate() const;
	};
}

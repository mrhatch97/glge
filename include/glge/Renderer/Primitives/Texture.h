#pragma once

#include <glge/Renderer/Resources/ResourceData.h>
#include <glge/Common.h>

namespace eng::render::primitive
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
		Texture(const res::TextureData & texture_data);

		~Texture();

		void activate() const;
	};
}
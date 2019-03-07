#include <glge/renderer/primitives/texture.h>

#include <gl_texture.h>

namespace glge::renderer::primitive
{
	Texture::Texture(const TextureData & texture_data) :
		texture(std::make_unique<TextureImpl>(texture_data))
	{
	}

	Texture::~Texture() = default;

	void Texture::activate() const
	{
		texture->activate();
	}
}

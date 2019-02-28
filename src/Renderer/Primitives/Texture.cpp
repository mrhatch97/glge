#include <glge/Renderer/Primitives/Texture.h>

#include <GLTexture.h>

namespace eng::render::primitive
{
	Texture::Texture(const res::TextureData & texture_data) :
		texture(std::make_unique<TextureImpl>(texture_data))
	{
	}

	Texture::~Texture() = default;

	void Texture::activate() const
	{
		texture->activate();
	}
}
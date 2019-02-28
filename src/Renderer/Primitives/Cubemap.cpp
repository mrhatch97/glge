#include "glge/Renderer/Primitives/Cubemap.h"

#include "GLCubemap.h"

namespace eng::render::primitive
{
	Cubemap::Cubemap(const res::CubemapData & cubemap_data) :
		cubemap(std::make_unique<CubemapImpl>(cubemap_data))
	{
	}

	Cubemap::~Cubemap() = default;

	void Cubemap::activate() const
	{
		cubemap->activate();
	}
}
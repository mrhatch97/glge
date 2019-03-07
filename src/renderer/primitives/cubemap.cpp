#include "glge/renderer/primitives/cubemap.h"

#include "gl_cubemap.h"

namespace glge::renderer::primitive
{
	Cubemap::Cubemap(const CubemapData & cubemap_data) :
		cubemap(std::make_unique<CubemapImpl>(cubemap_data))
	{
	}

	Cubemap::~Cubemap() = default;

	void Cubemap::activate() const
	{
		cubemap->activate();
	}
}

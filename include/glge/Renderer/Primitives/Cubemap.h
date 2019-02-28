#pragma once

#include <glge/Renderer/Resources/ResourceData.h>
#include <glge/Common.h>

namespace eng::render::primitive
{
	namespace opengl
	{
		class GLCubemap;
	}

	using CubemapImpl = opengl::GLCubemap;

	class Cubemap
	{
	private:
		uptr<CubemapImpl> cubemap;
	public:
		Cubemap(const res::CubemapData & cubemap_data);

		~Cubemap();

		void activate() const;
	};
}
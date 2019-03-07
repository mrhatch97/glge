#pragma once

#include <glge/common.h>
#include <glge/renderer/primitives/primitive_data.h>

namespace glge::renderer::primitive
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
		Cubemap(const CubemapData & cubemap_data);

		~Cubemap();

		void activate() const;
	};
}

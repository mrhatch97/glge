#pragma once

#include <glge/common.h>
#include <glge/renderer/primitives/primitive_data.h>

namespace glge::renderer::primitive
{
	class Cubemap
	{
	public:
		Cubemap() = default;

		virtual ~Cubemap() = default;

		virtual void activate() const = 0;
	
		static unique_ptr<Cubemap> from_file(const CubemapFileInfo & file_info);
	};
}

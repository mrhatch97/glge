#pragma once

#include <glge/common.h>

#include <glge/renderer/primitives/renderable.h>

namespace glge::renderer::primitive
{
	class Lines : public Renderable
	{
	public:
		Lines() = default;

		virtual ~Lines() = default;

		static unique_ptr<Lines> from_file(const vector<vec3> & points);
	};
}

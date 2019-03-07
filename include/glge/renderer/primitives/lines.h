#pragma once

#include <glge/common.h>

#include <glge/renderer/primitives/renderable.h>

namespace glge::renderer::primitive
{
	namespace opengl
	{
		class GLLines;
	}

	using LinesImpl = opengl::GLLines;

	class Lines : public Renderable
	{
	private:
		uptr<LinesImpl> lines;
	public:
		Lines(const Vector<vec3> & lines_data);

		void render() const override;

		~Lines();
	};
}

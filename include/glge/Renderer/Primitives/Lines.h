#pragma once

#include <glge/Common.h>

#include <glge/Renderer/Primitives/Renderable.h>

namespace eng::render::primitive
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
#include "glge/renderer/primitives/lines.h"

#include "gl_lines.h"

namespace glge::renderer::primitive
{
	Lines::Lines(const Vector<vec3> & lines_data) :
		lines(std::make_unique<LinesImpl>(lines_data))
	{
	}

	void Lines::render() const
	{
		lines->render();
	}

	Lines::~Lines() = default;
}

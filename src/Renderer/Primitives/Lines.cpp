#include "glge/Renderer/Primitives/Lines.h"

#include "GLLines.h"

namespace eng::render::primitive
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
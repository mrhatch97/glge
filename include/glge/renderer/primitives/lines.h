/// <summary>Support for lines in 3D space.</summary>
///
/// Contains class for using 3D lines.
///
/// \file lines.h

#pragma once

#include <glge/common.h>

#include <glge/renderer/primitives/renderable.h>

namespace glge::renderer::primitive
{
	/// <summary>
	/// Class representing a set of connected lines in 3D space.
	/// </summary>
	class Lines : public Renderable
	{
	public:
		Lines() = default;

		virtual ~Lines() = default;

		/// <summary>
		/// Load a set of lines using a collection of points interpreted as
		/// segment endpoints.
		/// </summary>
		/// <param name="points">Set of line segment endpoints.</param>
		/// <returns>Pointer to created Lines object.</returns>
		static unique_ptr<Lines> from_points(const vector<vec3> & points);
	};
}   // namespace glge::renderer::primitive

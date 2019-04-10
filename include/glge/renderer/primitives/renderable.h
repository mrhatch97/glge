/// <summary>Renderable interface.</summary>
///
/// \file renderable.h

#pragma once

namespace glge::renderer::primitive
{
  /// <summary>
  /// Abstract class representing renderable objects.
  /// </summary>
	class Renderable
	{
	public:
    /// <summary>
    /// Renders the object.
    /// </summary>
		virtual void render() const = 0;

		virtual ~Renderable() = default;
	};
}

/// <summary>Support for describing motion in a 3D space.</summary>
///
/// Contains structures and functions for describing and performing
/// movement in 3D space.
///
/// \file motion.h

#pragma once

#include <glge/common.h>

namespace glge::util
{
	/// <summary>
	/// Placement of an object in 3D space; position and
	/// orientation encoded in a homogeneous matrix.
	/// </summary>
	struct Placement
	{
		/// <summary>
		/// The matrix describing the 3D transform.
		/// </summary>
		/// The matrix describing the 3D transform, which
		/// takes an object at the origin in default
		/// orientation to the specified position and orientation.
		mat4 transform;

		Placement() noexcept : transform(1.0f) {}

		explicit Placement(mat4 transform) noexcept : transform(transform) {}

		Placement(const Placement & other) noexcept : transform(other.transform)
		{}

		/// <summary>
		/// Get the right orientation vector.
		/// </summary>
		/// <returns>vec3 representation of the right orientation
		/// vector.</returns>
		vec3 get_right_direction() const { return transform[0]; }

		/// <summary>
		/// Get the up orientation vector.
		/// </summary>
		/// <returns>vec3 representation of the up orientation vector.</returns>
		vec3 get_up_direction() const { return transform[1]; }

		/// <summary>
		/// Get the forward orientation vector.
		/// </summary>
		/// <returns>
		/// vec3 representation of the forward orientation vector.
		/// </returns>
		vec3 get_forward_direction() const { return transform[2]; }

		/// <summary>
		/// Get the position vector.
		/// </summary>
		/// <returns>
		/// vec3 representation of the offset from the origin.
		/// </returns>
		vec3 get_position() const { return transform[3]; }
	};
}   // namespace glge::util

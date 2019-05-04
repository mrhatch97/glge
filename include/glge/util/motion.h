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
	/// Static descriptors for a right-handed coordinate system.
	/// </summary>
	struct RightHandCoordinateSystem
	{
		/// <summary>
		/// The right (positive X) axis.
		/// </summary>
		struct Right
		{
			/// <summary>
			/// 1.0f if axis is in positive direction, else -1.0f.
			/// </summary>
			static constexpr float signum = 1.0f;

			/// <summary>
			/// Index of this axis' coordinate in 3-tuples.
			/// </summary>
			static constexpr size_t idx = 0;
		};

		/// <summary>
		/// The left (negative X) axis.
		/// </summary>
		struct Left
		{
			/// <summary>
			/// 1.0f if axis is in positive direction, else -1.0f.
			/// </summary>
			static constexpr float signum = -Right::signum;

			/// <summary>
			/// Index of this axis' coordinate in 3-tuples.
			/// </summary>
			static constexpr size_t idx = Right::idx;
		};

		/// <summary>
		/// The up (positive Y) axis.
		/// </summary>
		struct Up
		{
			/// <summary>
			/// 1.0f if axis is in positive direction, else -1.0f.
			/// </summary>
			static constexpr float signum = 1.0f;

			/// <summary>
			/// Index of this axis' coordinate in 3-tuples.
			/// </summary>
			static constexpr size_t idx = 1;
		};

		/// <summary>
		/// The down (negative Y) axis.
		/// </summary>
		struct Down
		{
			/// <summary>
			/// 1.0f if axis is in positive direction, else -1.0f.
			/// </summary>
			static constexpr float signum = -Up::signum;

			/// <summary>
			/// Index of this axis' coordinate in 3-tuples.
			/// </summary>
			static constexpr size_t idx = Up::idx;
		};

		/// <summary>
		/// The forward (positive Z) axis.
		/// </summary>
		struct Forward
		{
			/// <summary>
			/// 1.0f if axis is in positive direction, else -1.0f.
			/// </summary>
			static constexpr float signum = 1.0f;

			/// <summary>
			/// Index of this axis' coordinate in 3-tuples.
			/// </summary>
			static constexpr size_t idx = 2;
		};

		/// <summary>
		/// The back (negative Z) axis.
		/// </summary>
		struct Back
		{
			/// <summary>
			/// 1.0f if axis is in positive direction, else -1.0f.
			/// </summary>
			static constexpr float signum = -Forward::signum;

			/// <summary>
			/// Index of this axis' coordinate in 3-tuples.
			/// </summary>
			static constexpr size_t idx = Forward::idx;
		};

		/// <summary>
		/// The homogenous basis for this coordinate system.
		/// </summary>
		static const mat4 basis;
	};

	/// <summary>
	/// Type alias for the coordinate system used throughout the library.
	/// </summary>
	using CoordSys = RightHandCoordinateSystem;

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

		/// <summary>
		/// Constructs a Placement using the coordinate system's basis as its
		/// initial orientation.
		/// </summary>
		Placement() noexcept : transform(CoordSys::basis) {}

		/// <summary>
		/// Constructs a Placement with the given initial transform.
		/// </summary>
		/// <param name="transform>
		/// Initial transform for the Placement.
		/// </param>
		explicit Placement(mat4 transform) noexcept : transform(transform) {}

		/// <summary>
		/// Constructs a Placement by copying another Placement's transform.
		/// </summary>
		/// <param name="other">
		/// Placement to copy transform from.
		/// </param>
		Placement(const Placement & other) noexcept : transform(other.transform)
		{}

		/// <summary>
		/// Get the orientation vector corresponding to the given model-space axis,
        /// in world-space coordinates.
		/// </summary>
        /// <typeparam name="AxisT">
        /// Directional axis; must be one of the types specified in a CoordinateSystem struct.
        /// </typeparam>
		template<typename AxisT>
		vec3 get_direction() const
		{
			return AxisT::signum * transform[AxisT::idx];
		}

		/// <summary>
		/// Get the right orientation vector.
		/// </summary>
		/// <returns>
		/// vec3 representation of the right orientation vector.
		/// </returns>
		vec3 get_right_direction() const
		{
			return get_direction<CoordSys::Right>();
		}

		/// <summary>
		/// Get the up orientation vector.
		/// </summary>
		/// <returns>vec3 representation of the up orientation vector.</returns>
		vec3 get_up_direction() const { return get_direction<CoordSys::Up>(); }

		/// <summary>
		/// Get the forward orientation vector.
		/// </summary>
		/// <returns>
		/// vec3 representation of the forward orientation vector.
		/// </returns>
		vec3 get_forward_direction() const
		{
			return get_direction<CoordSys::Forward>();
		}

		/// <summary>
		/// Get the position vector.
		/// </summary>
		/// <returns>
		/// vec3 representation of the offset from the origin.
		/// </returns>
		vec3 get_position() const { return transform[3]; }
	};
}   // namespace glge::util

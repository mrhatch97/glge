/// <summary>Representations of cameras for rendering.</summary>
///
/// Contains data structures for representing cameras for rendering
/// with utility functions for camera-related computations.
///
/// \file camera.h

#pragma once

#include <glge/common.h>
#include <glge/util/math.h>
#include <glge/util/motion.h>

namespace glge::renderer
{
	/// <summary>
	/// A set of descriptors for the intrinsic properties of a camera.
	/// </summary>
	struct CameraIntrinsics
	{
		/// <summary>
		/// Vertical Field of View; how high the view area is, in degrees.
		/// </summary>
		float v_fov;
		/// <summary>Aspect ratio; ratio of width to height of image.</summary>
		float aspect_ratio;
		/// <summary>Distance to the camera's near clip plane.</summary>
		float near_distance;
		/// <summary>Distance to the camera's far clip plane.</summary>
		float far_distance;

		/// <summary>
		/// Set the aspect ratio of the camera using width and
		/// height.
		/// </summary>
		/// <param name="width">Width of the image.</param>
		/// <param name="height">Height of the image.</param>
		void set_aspect_ratio(float width, float height)
		{
			aspect_ratio = width / height;
		}

		/// <summary>
		/// Computes the dimensions of the near clip plane.
		/// </summary>
		/// <returns>Near clip plane dimensions as (width, height).</returns>
		vec2 near_dimensions() const;

		/// <summary>
		/// Computes the dimensions of the far clip plane.
		/// </summary>
		/// <returns>Far clip plane dimensions as (width, height).</returns>
		vec2 far_dimensions() const;

		/// <summary>
		/// Computes the Projection matrix for this camera.
		/// </summary>
		/// <returns>4x4 projection matrix.</returns>
		mat4 get_P() const;
	};

	/// <summary>
	/// Representation of a camera in 3D space.
	/// </summary>
	struct Camera
	{
		/// <summary>
		/// The intrinsic properties of the camera.
		/// </summary>
		CameraIntrinsics intrinsics;

		/// <summary>
		/// The placement of the camera in 3D space.
		/// </summary>
		util::Placement placement;

		/// <summary>
		/// Construct a new Camera with the given intrinsic properties and
		/// placement.
		/// </summary>
		Camera(const CameraIntrinsics & intrinsics,
			   const util::Placement & placement);

		/// <summary>
		/// Computes the View matrix for this camera.
		/// </summary>
		/// <returns>4x4 view matrix.</returns>
		mat4 get_V() const;

		/// <summary>
		/// Computes the view frustum of this camera.
		/// </summary>
		/// <returns>View frustum of the camera.</returns>
		math::Frustum get_view_frustum() const;
	};
}   // namespace glge::renderer

#pragma once

#include <glge/common.h>
#include <glge/util/motion.h>
#include <glge/util/math.h>

namespace glge::renderer
{
	struct CameraIntrinsics
	{
		float v_fov;
		float aspect_ratio;
		float near_distance;
		float far_distance;

		void set_aspect_ratio(float width, float height)
		{
			aspect_ratio = width / height;
		}

		vec2 near_dimensions() const;
		vec2 far_dimensions() const;

		mat4 get_P() const;
	};

	struct Camera
	{
		CameraIntrinsics intrinsics;
		util::Placement placement;

		Camera(
			const CameraIntrinsics & intrinsics,
			const util::Placement & placement
		);

		mat4 get_V() const;

		math::Frustum get_view_frustum() const;
	};
}

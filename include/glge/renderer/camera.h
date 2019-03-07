#pragma once

#include <glge/common.h>
#include <glge/util/motion.h>
#include <glge/util/math.h>

namespace glge::util
{
	struct CameraIntrinsics
	{
		float fov;
		float ratio;
		float nearDist;
		float farDist;

		CameraIntrinsics() :
			fov(45.0f), ratio(0), nearDist(0.1f), farDist(10000.0f)
		{ }

		void set_ratio(int width, int height);

		vec2 near_dimensions() const;
		vec2 far_dimensions() const;

		mat4 get_P() const;
	};

	struct Camera
	{
		CameraIntrinsics intrinsics;
		Placement placement;

		mat4 get_V() const;

		math::Frustum get_view_frustum() const;
		std::array<math::Plane, 6> get_view_planes() const;
	};
}

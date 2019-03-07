#include "glge/renderer/camera.h"

namespace glge::util
{
	vec2 CameraIntrinsics::near_dimensions() const
	{
		auto height = 2 * glm::tan(fov / 2.0f) * nearDist;

		return vec2(height * ratio, height);
	}

	vec2 CameraIntrinsics::far_dimensions() const
	{
		auto height = 2 * glm::tan(fov / 2.0f) * farDist;

		return vec2(height * ratio, height);
	}
}

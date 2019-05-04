#include <glge/renderer/camera.h>

#include "test_utils.h"

namespace glge::test
{
	using namespace glge::renderer;

	void test_clip_planes()
	{
		CameraIntrinsics intrinsics{math::Degrees(45.0f), 1920.0f / 1080.0f,
									0.1f, 10000.0f};

		vec2 near_dim = intrinsics.near_dimensions();
		vec2 far_dim = intrinsics.far_dimensions();

		test_assert(vec_eq(near_dim, vec2(0.14727594f, 0.0828427151f)));
		test_assert(vec_eq(far_dim, vec2(14727.5938f, 8284.27148f)));
	}

	void test_V_matrix_trivial()
	{
		Camera cam(CameraIntrinsics{math::Degrees(45.0f), 1920.0f / 1080.0f,
									0.1f, 10000.0f},
				   util::Placement{mat4(1.0f)});

		test_assert(mat4_eq(cam.get_V(), mat4(1.0f)),
					"Trivial V matrix was incorrect");
	}

	void test_V_matrix()
	{
		Camera cam(CameraIntrinsics{math::Degrees(45.0f), 1920.0f / 1080.0f,
									0.1f, 10000.0f},
				   util::Placement{mat4(1.0f)});

		auto rotation_matrix = glm::toMat4(
			glm::angleAxis(glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f)));

		cam.placement.transform = rotation_matrix * cam.placement.transform;

		cam.placement.transform =
			glm::translate(vec3(-5.0f, 3.0f, -10.0f)) * cam.placement.transform;

		test_assert(mat4_eq(glm::inverse(cam.placement.transform), cam.get_V()),
					"V matrix was not equal to transform inverse");
	}

	void test_view_frustum()
	{
		Camera cam(CameraIntrinsics{math::Degrees(45.0f), 1920.0f / 1080.0f,
									0.1f, 10000.0f},
				   util::Placement());

		auto frustum = cam.get_view_frustum();

		math::Plane expected_near(vec3(0.0f, 0.0f, 0.1f),
								  vec3(0.0f, 0.0f, 1.0f));

		test_assert(expected_near == frustum.near, "Near plane was incorrect");

		math::Plane expected_far(vec3(0.0f, 0.0f, 10000.0f),
								 vec3(0.0f, 0.0f, -1.0f));

		test_assert(expected_far == frustum.far, "Far plane was incorrect");

		auto near_center = cam.placement.get_position() +
						   cam.placement.get_forward_direction() *
							   cam.intrinsics.near_distance;

		// Compute direction vector to the near-right corner of the frustum
		auto right_corner = glm::normalize(
			near_center + cam.placement.get_right_direction() *
							  cam.intrinsics.near_dimensions().x / 2.0f);

		// Compute normal vector by crossing with camera up-vector; inverse to
		// point towards inside of frustum
		auto right_normal =
			-glm::cross(cam.placement.get_up_direction(), right_corner);

		math::Plane expected_right(vec3(0.0f, 0.0f, 0.0f), right_normal);

		test_assert(expected_right == frustum.right,
					"Right plane was incorrect");

		auto left_corner = glm::normalize(
			near_center - cam.placement.get_right_direction() *
							  cam.intrinsics.near_dimensions().x / 2.0f);

		auto left_normal =
			glm::cross(cam.placement.get_up_direction(), left_corner);

		math::Plane expected_left(vec3(0.0f, 0.0f, 0.0f), left_normal);

		test_assert(expected_left == frustum.left, "Left plane was incorrect");

		auto top_corner = glm::normalize(
			near_center + cam.placement.get_up_direction() *
							  cam.intrinsics.near_dimensions().y / 2.0f);

		auto top_normal =
			glm::cross(cam.placement.get_right_direction(), top_corner);

		math::Plane expected_top(vec3(0.0f, 0.0f, 0.0f), top_normal);

		test_assert(expected_top == frustum.top, "Top plane was incorrect");

		auto bottom_corner = glm::normalize(
			near_center - cam.placement.get_up_direction() *
							  cam.intrinsics.near_dimensions().y / 2.0f);

		auto bottom_normal =
			-glm::cross(cam.placement.get_right_direction(), bottom_corner);

		math::Plane expected_bottom(vec3(0.0f, 0.0f, 0.0f), bottom_normal);

		test_assert(expected_bottom == frustum.bottom, "Bottom plane was incorrect");
	}
}   // namespace glge::test

using namespace glge::test;

int main()
{
	Test(test_clip_planes).run();
	Test(test_V_matrix_trivial).run();
	Test(test_V_matrix).run();
	Test(test_view_frustum).run();
}

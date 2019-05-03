#include <glge/renderer/camera.h>

#include "test_utils.h"

namespace glge::test
{
	using namespace glge::renderer;

	void test_clip_planes()
	{
		CameraIntrinsics intrinsics{45.0f, 1920.0f / 1080.0f, 0.1, 10000.0f};

		vec2 near_dim = intrinsics.near_dimensions();
		vec2 far_dim = intrinsics.far_dimensions();

		test_assert(near_dim == vec2(0.14727594f, 0.0828427151f));
		test_assert(far_dim == vec2(14727.5938f, 8284.27148f));
	}

	void test_V_matrix_trivial()
	{
		Camera cam(CameraIntrinsics{45.0f, 1920.0f / 1080.0f, 0.1f, 10000.0f},
				   util::Placement{mat4(1.0f)});

		test_assert(cam.get_V() == mat4(1.0f),
					"Trivial V matrix was incorrect");
	}

	void test_V_matrix()
	{
		Camera cam(CameraIntrinsics{45.0f, 1920.0f / 1080.0f, 0.1f, 10000.0f},
				   util::Placement{mat4(1.0f)});

		cam.placement.transform =
			glm::translate(vec3(-5.0f, 3.0f, -10.0f)) * cam.placement.transform;

		test_assert(glm::inverse(cam.placement.transform) == cam.get_V(),
					"V matrix was not equal to transform inverse");
	}

	void test_view_frustum()
	{
		Camera cam(CameraIntrinsics{45.0f, 1920.0f / 1080.0f, 0.1f, 10000.0f},
				   util::Placement{mat4(1.0f)});

    auto frustum = cam.get_view_frustum();

    // eyy it didn't crash
	}
}   // namespace glge::test

using namespace glge::test;

int main()
{
	Test(test_clip_planes).run();
	Test(test_V_matrix_trivial).run();
	Test(test_V_matrix).run();
}

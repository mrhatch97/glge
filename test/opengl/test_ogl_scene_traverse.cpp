#include <glge/renderer/primitives/model.h>
#include <glge/renderer/primitives/shader_program.h>
#include <glge/renderer/render_settings.h>
#include <glge/renderer/renderer.h>
#include <glge/renderer/scene_graph/scene.h>

#include "ogl_test_utils.h"

using namespace glge;
using namespace glge::renderer;
using namespace glge::renderer::primitive;
using namespace glge::renderer::scene_graph;

namespace glge::test::opengl::cases
{
    /// <summary>
    /// Context for Scene traversal tests.
    /// </summary.
	class SceneTraverseTest : public OGLTest
	{
	public:
        /// \test Tests that a Scene can be traversed and produces a
        /// Renderer with the expected number of targets.
		void test_traverse()
		{
			auto color_shader = ColorShader::load();
			auto color_instance =
				color_shader->instance(vec3(1.0f, 0.0f, 0.0f));

			auto model = Model::from_file(
				ModelFileInfo{"./resources/models/test.obj"});

			Scene scene;

			auto root_handle = scene.get_root_handle();
			root_handle.add_geometry(*model, color_instance);
			auto camera_handle = root_handle.add_camera(CameraIntrinsics());
			camera_handle.activate();

			auto renderer = scene.prepare_renderer();

			test_assert(renderer.target_count() == 1,
						"Expected number of targets was not met");
		}
	};
}   // namespace glge::test::opengl::cases


int main()
{
    using glge::test::Test;
    using glge::test::opengl::cases::SceneTraverseTest;

    Test::run(&SceneTraverseTest::test_traverse);
}

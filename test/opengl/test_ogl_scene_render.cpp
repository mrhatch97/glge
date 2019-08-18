#include <glge/renderer/primitives/model.h>
#include <glge/renderer/primitives/shader_program.h>
#include <glge/renderer/render_settings.h>
#include <glge/renderer/renderer.h>
#include <glge/renderer/scene_graph/scene.h>

#include "ogl_test_utils.h"

namespace glge::test::opengl::cases
{
	using namespace glge::renderer;
	using namespace glge::renderer::primitive;
	using namespace glge::renderer::scene_graph;

    /// <summary>
    /// Context for Scene rendering tests.
    /// </summary>
	class SceneRenderTest : public OGLTest
	{
	public:
        /// \test Tests whether a Scene can be successfully rendered by a
        /// Renderer.
		void test_render()
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

			renderer.render();
		}
	};

}   // namespace glge::test::opengl::cases

int main()
{
    using glge::test::Test;
    using glge::test::opengl::cases::SceneRenderTest;

	Test::run(&SceneRenderTest::test_render);
}

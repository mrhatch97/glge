#include <glge/renderer/primitives/model.h>
#include <glge/renderer/renderer.h>

#include "ogl_test_utils.h"

namespace glge::test::opengl::cases
{
	using namespace glge::renderer::primitive;

    /// <summary>Context for Model load tests.</summary>
	class ModelLoadTest : public OGLTest
	{
	public:
        /// \test Tests that a Model can be loaded into VRAM from a file
        /// on disk.
		void test_load()
		{
			Model::from_file(ModelFileInfo{"./resources/models/test.obj",
										   ModelFiletype::Auto});
		}
	};

}   // namespace glge::test::opengl::cases

int main()
{
	using glge::test::Test;
	using glge::test::opengl::cases::ModelLoadTest;

	Test::run(&ModelLoadTest::test_load);
}

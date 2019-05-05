#include <glge/renderer/primitives/cubemap.h>
#include <glge/renderer/renderer.h>

#include "ogl_test_utils.h"

using namespace glge::renderer::primitive;

namespace glge::test::opengl::cases
{
    /// <summary>Context for Cubemap load tests.</summary>
	class CubemapLoadTest : public OGLTest
	{
	public:
		/// \test Tests that a Cubemap can be loaded into VRAM from
		/// a set of files on disk.
		void test_load()
		{
			Cubemap::from_file(
				CubemapFileInfo{"./resources/cubemaps/test_up.tga",
								"./resources/cubemaps/test_dn.tga",
								"./resources/cubemaps/test_lf.tga",
								"./resources/cubemaps/test_rt.tga",
								"./resources/cubemaps/test_ft.tga",
								"./resources/cubemaps/test_bk.tga"});
		}
	};
}   // namespace glge::test::opengl::cases


int main()
{
	using glge::test::Test;
	using namespace glge::test::opengl::cases;

	Test::run(&CubemapLoadTest::test_load);
}

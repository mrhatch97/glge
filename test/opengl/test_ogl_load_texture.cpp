#include <glge/renderer/primitives/texture.h>
#include <glge/renderer/renderer.h>

#include "ogl_test_utils.h"

using namespace glge::renderer::primitive;

namespace glge::test::opengl::cases
{
    /// <summary>Context for Texture load tests.</summary>
	class TextureLoadTest : public OGLTest
	{
	public:
		/// \test Tests whether a Texture can be loaded from a .png file on
		/// disk.
		void test_load()
		{
			Texture::from_file(
				TextureFileInfo{"./resources/textures/test.png"});
		}
	};
}   // namespace glge::test::opengl::cases


int main()
{
	using glge::test::Test;
	using glge::test::opengl::cases::TextureLoadTest;

	Test::run(&TextureLoadTest::test_load);
}

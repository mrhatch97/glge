#include <glge/renderer/renderer.h>

#include "ogl_test_utils.h"

namespace glge::test::opengl::cases
{
	/// \test Tests that the OpenGL environment (GLEW, etc.) is
	/// able to be created and configured without error.
	void test_configure()
	{
		auto context = create_ogl_context();

		renderer::configure_environment();
	}
}   // namespace glge::test::opengl::cases


int main()
{
	using glge::test::Test;
	using namespace glge::test::opengl::cases;

	Test::run(test_configure);
}

#include <glge/renderer/renderer.h>

#include "ogl_test_utils.h"

using namespace glge;

void test_configure()
{
	auto context = create_ogl_context();

	renderer::configure_environment();
}

int main()
{
	Test(test_configure).run();
}

#include <glge/renderer/primitives/texture.h>
#include <glge/renderer/renderer.h>

#include "ogl_test_utils.h"

using namespace glge;
using namespace glge::renderer::primitive;

void test_load()
{
	Texture::from_file(TextureFileInfo{ "./resources/textures/test.png" });
}

int main()
{
	OGLTest(test_load).run();
}
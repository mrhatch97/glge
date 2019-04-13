#include <glge/renderer/primitives/cubemap.h>
#include <glge/renderer/renderer.h>

#include "ogl_test_utils.h"

using namespace glge;
using namespace glge::renderer::primitive;

void test_load()
{
	Cubemap::from_file(CubemapFileInfo{
		"./resources/cubemaps/test_up.tga", "./resources/cubemaps/test_dn.tga",
		"./resources/cubemaps/test_lf.tga", "./resources/cubemaps/test_rt.tga",
		"./resources/cubemaps/test_ft.tga",
		"./resources/cubemaps/test_bk.tga"});
}

int main()
{
	OGLTest(test_load).run();
}
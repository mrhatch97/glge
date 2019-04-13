#include <glge/renderer/primitives/model.h>
#include <glge/renderer/renderer.h>

#include "ogl_test_utils.h"

using namespace glge;
using namespace glge::renderer::primitive;

void test_load()
{
	Model::from_file(ModelFileInfo{"./resources/models/test.obj", true});
}

int main()
{
	OGLTest(test_load).run();
}
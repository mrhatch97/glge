#include <glge/renderer/primitives/primitive_data.h>

#include "test_utils.h"

using namespace glge;
using namespace glge::renderer::primitive;

void test_load()
{
	ModelData data = ModelData::from_file(ModelFileInfo{ "./resources/models/test.obj", true });

	test_equal(123U, data.vertex_data.points.size());
	test_equal(122U, data.normal_data.points.size());
	test_equal(448U, data.uv_data.points.size());
	test_equal(178U * 3, data.vertex_data.indices.size());
	test_equal(178U * 3, data.normal_data.indices.size());
	test_equal(178U * 3, data.uv_data.indices.size());
}

int main()
{
	Test(test_load).run();
}
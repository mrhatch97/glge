#include <glge/renderer/primitives/primitive_data.h>

#include "test_utils.h"

using namespace glge;
using namespace glge::renderer::primitive;

void test_load()
{
	ModelData data = ModelData::from_file(
		ModelFileInfo{"./resources/models/big.obj", false});

	test_equal(34835U, data.vertex_data.points.size());
	test_equal(34835U, data.normal_data.points.size());
	test_equal(0U, data.uv_data.points.size());
	test_equal(69666U * 3, data.vertex_data.indices.size());
	test_equal(69666U * 3, data.normal_data.indices.size());
	test_equal(0U, data.uv_data.indices.size());
}

int main()
{
	Test(test_load).run();
}
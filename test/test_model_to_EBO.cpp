#include <glge/renderer/primitives/primitive_data.h>

#include "test_utils.h"

using namespace glge;
using namespace glge::renderer::primitive;

void test_move()
{
	ModelData data = ModelData::from_file(ModelFileInfo{ "./resources/models/test.obj", true });

	EBOModelData ebo_data = ModelData::to_EBO_data(std::move(data));

	test_equal(178U * 3, ebo_data.indices.size());
	test_equal(ebo_data.indices.size(), ebo_data.vertices.size());
	test_equal(ebo_data.indices.size(), ebo_data.normals.size());
	test_equal(ebo_data.indices.size(), ebo_data.uvs.size());
}

void test_copy()
{
	ModelData data = ModelData::from_file(ModelFileInfo{ "./resources/models/test.obj", true });

	EBOModelData ebo_data = ModelData::to_EBO_data(data);

	test_equal(178U * 3, ebo_data.indices.size());
	test_equal(ebo_data.indices.size(), ebo_data.vertices.size());
	test_equal(ebo_data.indices.size(), ebo_data.normals.size());
	test_equal(ebo_data.indices.size(), ebo_data.uvs.size());
}

int main()
{
	Test(test_copy).run();
	Test(test_move).run();
}
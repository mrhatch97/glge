#include <glge/renderer/primitives/primitive_data.h>

#include "test_utils.h"

namespace glge::test::cases
{
	using namespace glge::renderer::primitive;

	/// \test Tests that the correct EBO data is produced by the
	/// move overload of to_EBO_data.
	void test_move()
	{
		ModelData data = ModelData::from_file(
			ModelFileInfo{"./resources/models/test.obj", true});

		EBOModelData ebo_data = ModelData::to_EBO_data(std::move(data));

		test_equal(178U * 3, ebo_data.indices.size());
		test_equal(ebo_data.indices.size(), ebo_data.vertices.size());
		test_equal(ebo_data.indices.size(), ebo_data.normals.size());
		test_equal(ebo_data.indices.size(), ebo_data.uvs.size());
	}

	/// \test Tests that the correct EBO data is produced by the
	/// copy overload of to_EBO_data.
	void test_copy()
	{
		ModelData data = ModelData::from_file(
			ModelFileInfo{"./resources/models/test.obj", true});

		EBOModelData ebo_data = ModelData::to_EBO_data(data);

		test_equal(178U * 3, ebo_data.indices.size());
		test_equal(ebo_data.indices.size(), ebo_data.vertices.size());
		test_equal(ebo_data.indices.size(), ebo_data.normals.size());
		test_equal(ebo_data.indices.size(), ebo_data.uvs.size());
	}
}   // namespace glge::test::cases

int main()
{
	using glge::test::Test;
	using namespace glge::test::cases;

	Test::run(test_copy);
	Test::run(test_move);
}

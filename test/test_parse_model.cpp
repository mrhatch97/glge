#include <glge/renderer/primitives/primitive_data.h>

#include "test_utils.h"

namespace glge::test::cases
{
	using namespace glge::renderer::primitive;

    /// \test Tests whether a ModelData can be loaded from an .obj file
    /// on disk.
	void test_load()
	{
		ModelData data = ModelData::from_file(
			ModelFileInfo{"./resources/models/test.obj"});

		test_equal(123U, data.vertex_data.points.size());
		test_equal(122U, data.normal_data.points.size());
		test_equal(448U, data.uv_data.points.size());
		test_equal(178U * 3, data.vertex_data.indices.size());
		test_equal(178U * 3, data.normal_data.indices.size());
		test_equal(178U * 3, data.uv_data.indices.size());
	}
}   // namespace glge::test::cases


int main()
{
	using glge::test::Test;
	using namespace glge::test::cases;

    Test::run(test_load);
}

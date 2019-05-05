#include <glge/renderer/primitives/primitive_data.h>

#include "test_utils.h"

namespace glge::test::cases
{
	using namespace glge::renderer::primitive;

    /// \test Tests whether a ModelData can be loaded from a large
    /// untextured .obj file on disk. Useful for benchmarking.
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
}   // namespace glge::test::cases


int main()
{
	using glge::test::Test;
	using namespace glge::test::cases;

    Test::run(test_load);
}

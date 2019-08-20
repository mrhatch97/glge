#include <glge/model_parser/model_parser.h>

#include "test_utils.h"

#include <cstdio>
#include <cstring>

namespace glge::test::cases
{
	using namespace glge::model_parser;

	/// <summary>
	/// Context for packed model write/load tests.
	/// </summary>
	class PackedModelTest : public Test
	{
	private:
		ModelData data;

		constexpr static auto packed_filepath = "./resources/models/packed.pck";

	public:
		void pre_test() override
		{
			data = ModelData::from_file(ModelFileInfo{
				"./resources/models/big.obj", ModelFiletype::Auto});
		}

		/// \test Tests whether a ModelData loaded from a file on disk
		/// can be written to a packed file then re-read without loss
		/// of data.
		void test_write_read()
		{
			write_packed_file(packed_filepath, data);

			ModelData packed_data = ModelData::from_file(
				ModelFileInfo{packed_filepath, ModelFiletype::Auto});

			test_assert(vector_eq(data.vertex_data.points,
								  packed_data.vertex_data.points));
			test_assert(vector_eq(data.normal_data.points,
								  packed_data.normal_data.points));
			test_assert(
				vector_eq(data.uv_data.points, packed_data.uv_data.points));
			test_assert(vector_eq(data.vertex_data.indices,
								  packed_data.vertex_data.indices));
			test_assert(vector_eq(data.normal_data.indices,
								  packed_data.normal_data.indices));
			test_assert(
				vector_eq(data.uv_data.indices, packed_data.uv_data.indices));
		}

		void post_test() override
		{
			if (!std::remove(packed_filepath))
			{
				throw std::runtime_error("Failed to delete packed file!");
			}
		}
	};
}   // namespace glge::test::cases


int main()
{
	using glge::test::Test;
	using namespace glge::test::cases;

	Test::run(&PackedModelTest::test_write_read);
}

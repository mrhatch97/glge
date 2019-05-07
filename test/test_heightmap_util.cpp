#include <glge/util/heightmap.h>
#include <glge/util/util.h>

#include "test_utils.h"

#include <algorithm>

namespace glge::test::cases
{
	using namespace glge::util;

	/// \test Tests that the correct number of indices is returned by 
	/// glge::util::heightmap_indices.
	void test_indices()
	{
		const size_t width = 1025, height = 1025;
		const size_t num_triangles = (width - 1) * (height - 1) * 2;

		auto indices = heightmap_indices(width, width);

		test_equal(num_triangles * 3, indices.size());
	}

	/// \test Tests that the correct number of valid normals is returned
	/// by glge::util::heightmap_normals.
	void test_normals()
	{
		vector<vec3> heightmap{{0, 0, 0.5f},  {1, 0, 0.3f}, {2, 0, 0.4f},
							   {0, 1, -0.2f}, {1, 1, 3.5f}, {2, 1, 2.8f},
							   {0, 2, 5.5f},  {1, 2, 1.5f}, {2, 2, -4.1f}};

		auto indices = heightmap_indices(3, 3);

		auto normals = heightmap_normals(heightmap, indices);

		test_equal(9, normals.size());

		auto check_fn = [](const vec3 & v) {
			test_assert(float_eq(glm::length(v), 1.0f),
						"Length was not normal");
		};

		std::for_each(normals.cbegin(), normals.cend(), check_fn);
	}

	/// \test Tests that the correct number of valid uvs is returned
	/// by glge::util::heightmap_uvs.
	void test_uvs()
	{
		const size_t width = 1025, height = 1025;

		auto uvs = heightmap_uvs(width, height);

		test_equal(width * height, uvs.size());

		auto valid_uv = [](const vec2 & uv) {
			return uv.x >= 0.0f && uv.x <= 1.0f && uv.y >= 0.0f && uv.y <= 1.0f;
		};

		auto check_fn = [valid_uv](const vec2 & v) {
			test_assert(valid_uv(v), "uv was not valid");
		};

		std::for_each(uvs.cbegin(), uvs.cend(), check_fn);
	}
}   // namespace glge::test::cases

int main()
{
	using glge::test::Test;
	using namespace glge::test::cases;

	Test::run(test_indices);
	Test::run(test_normals);
	Test::run(test_uvs);
}

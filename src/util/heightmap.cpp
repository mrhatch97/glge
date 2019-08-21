#include "glge/util/heightmap.h"

#include <internal/util/_compat.h>
#include <internal/util/_util.h>

namespace glge::util
{
	vector<unsigned int> heightmap_indices(size_t width, size_t height)
	{
		// two triangles per grid quad
		const size_t num_triangles = (width - 1) * (height - 1) * 2;

		// 3 indices per triangle
		vector<unsigned int> indices(num_triangles * 3);

		unsigned int index = 0;

		auto i_height = util::safe_cast<size_t, unsigned>(height);
		auto i_width = util::safe_cast<size_t, unsigned>(width);

		for (unsigned int z = 0; z < (i_height - 1); z++)
		{
			for (unsigned int x = 0; x < (i_width - 1); x++)
			{
				unsigned int vertex_num = z * i_width + x;

				// upper triangle
				indices.at(index++) = vertex_num;
				indices.at(index++) = vertex_num + i_width + 1;
				indices.at(index++) = vertex_num + 1;

				// lower triangle
				indices.at(index++) = vertex_num;
				indices.at(index++) = vertex_num + i_width;
				indices.at(index++) = vertex_num + i_width + 1;
			}
		}

		return indices;
	}

	// Computes the vertex normals for a heightmap with the given indices
	vector<vec3> heightmap_normals(const vector<vec3> & heightmap,
								   const vector<unsigned int> & indices)
	{
		vector<vec3> normals(heightmap.size());

		for (size_t i = 0; i < indices.size(); i += 3)
		{
			vec3 v0 = heightmap.at(indices.at(i + 0));
			vec3 v1 = heightmap.at(indices.at(i + 1));
			vec3 v2 = heightmap.at(indices.at(i + 2));

			vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

			normals.at(indices.at(i + 0)) = normal;
			normals.at(indices.at(i + 1)) += normal;
			normals.at(indices.at(i + 2)) += normal;
		}

		std::transform(EXECUTION_POLICY_PAR_UNSEQ normals.cbegin(),
					   normals.cend(), normals.begin(),
					   [](const vec3 & v) { return glm::normalize(v); });

		return normals;
	}

	vector<vec2> heightmap_uvs(size_t width, size_t height)
	{
		vector<vec2> uvs(width * height);

		for (size_t z = 0; z < (height - 1); z++)
		{
			for (size_t x = 0; x < (width - 1); x++)
			{
				float z_f = static_cast<float>(z);
				float x_f = static_cast<float>(x);

				uvs[z * height + x] = vec2(z_f / width, x_f / height);
			}
		}

		return uvs;
	}
}   // namespace glge::util

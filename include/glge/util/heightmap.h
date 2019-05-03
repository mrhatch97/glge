#pragma once

#include <glge/common.h>
#include <glge/util/compat.h>
#include <glge/util/util.h>

namespace glge::util
{
	// TODO convert this to constexpr
	/// <summary>
	/// Converts a heightmap with the given dimensions to a list of
	/// indices describing the heightmap as a mesh of triangles.
	/// </summary>
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

	vector<vec2>
	heightmap_uvs(const vector<vec3> & heightmap, size_t width, size_t height)
	{
		vector<vec2> uvs(heightmap.size());

		std::transform(
			EXECUTION_POLICY_PAR_UNSEQ heightmap.cbegin(), heightmap.cend(),
			uvs.begin(),
			// HACK get rid of the hacky constant - controls the extent to which
			// textures are tiled over the terrain (higher - more tiling)
			[=](const vec3 & v) {
				return vec2(v.x / width / 60.0f, v.z / height / 60.0f);
			});

		return uvs;
	}
}   // namespace glge::util

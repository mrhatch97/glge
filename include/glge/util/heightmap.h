#pragma once

#include <glge/common.h>

namespace glge::util
{
	// TODO convert this to constexpr
	/// <summary>
	/// Converts a heightmap with the given dimensions to a list of
	/// indices describing the heightmap as a mesh of triangles.
	/// </summary>
	vector<unsigned int> heightmap_indices(size_t width, size_t height);

	// Computes the vertex normals for a heightmap with the given indices
	vector<vec3> heightmap_normals(const vector<vec3> & heightmap,
								   const vector<unsigned int> & indices);

	vector<vec2>
	heightmap_uvs(const vector<vec3> & heightmap, size_t width, size_t height);
}   // namespace glge::util

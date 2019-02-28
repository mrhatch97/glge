#pragma once

#include <glge/Common.h>
#include <glge/Util/Util.h>

namespace eng::procgen
{
		Vector<unsigned int> heightmap_indices(const size_t width, const size_t height);
		Vector<vec3> heightmap_normals(const Vector<vec3> & heightmap, const Vector<unsigned int> indices);
		Vector<vec2> heightmap_uvs(const Vector<vec3> & heightmap, const size_t width, const size_t height);
}
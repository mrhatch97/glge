#include <glge/util/heightmap.h>
#include <glge/util/util.h>

#include "test_utils.h"

namespace glge::test
{
	using namespace glge::util;

	void test_indices() 
    {
        heightmap_indices(1025, 1025);
    }

	void test_normals() {}

	void test_uvs() {}
}   // namespace glge::test

using namespace glge::test;


int main()
{
	Test(test_indices).run();
	Test(test_normals).run();
	Test(test_uvs).run();
}

#include <glge/Util/Math.h>

#include "test_utils.h"

using namespace eng;
using namespace eng::math;

int main()
{
	Plane p(vec3(0, 0, 0), vec3(0, 1, 0));

	test_assert(contains(p, Sphere{ 1.0f, vec3(5, 1, -2) }), "Sphere should be inside plane");
	test_assert(contains(p, Sphere{ 3.0f, vec3(0, -2, 0) }), "Sphere should be inside plane");
	test_assert(!contains(p, Sphere{ 2.0f, vec3(0, -3, 5) }), "Sphere should not be inside frustum");

	Frustum f(
		{ 
			Plane(vec3(0, 0, -2), vec3(0, 0, 1)), 
			Plane(vec3(0, 0, 2), vec3(0, 0, -1)),
			Plane(vec3(2, 0, 0), vec3(-1, 0, 0)),
			Plane(vec3(-2, 0, 0), vec3(1, 0, 0)),
			Plane(vec3(0, 2, 0), vec3(0, -1, 0)),
			Plane(vec3(0, -2, 0), vec3(0, 1, 0))
		}
	);

	test_assert(contains(f, Sphere{ 1.0f, vec3(0, 0, 0) }), "Sphere should be inside frustum");
	test_assert(contains(f, Sphere{ 3.0f, vec3(0, 0, 0) }), "Sphere should be inside frustum");
	test_assert(contains(f, Sphere{ 3.0f, vec3(0, 4, 0) }), "Sphere should be inside frustum");
	test_assert(!contains(f, Sphere{ 1.0f, vec3(0, 4, 0) }), "Sphere should not be inside frustum");

	return 0;
}
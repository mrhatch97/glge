#include <glge/util/motion.h>

#include "test_utils.h"

namespace glge::test
{
	void test_default()
	{
		util::Placement placement;

		test_assert(vec_eq(vec3(0.0f, 0.0f, 0.0f), placement.get_position()),
					"Default position was incorrect");

		test_assert(
			vec_eq(vec3(1.0f, 0.0f, 0.0f), placement.get_right_direction()),
			"Default right vector was incorrect");
		test_assert(
			vec_eq(vec3(0.0f, 1.0f, 0.0f), placement.get_up_direction()),
			"Default up vector was incorrect");
		test_assert(
			vec_eq(vec3(0.0f, 0.0f, 1.0f), placement.get_forward_direction()),
			"Default forward vector was incorrect");
	}

	void test_move()
	{
		util::Placement placement;

		// Test rotation

		auto rotation_matrix = glm::toMat4(
			glm::angleAxis(glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f)));

		placement.transform = rotation_matrix * placement.transform;

		test_assert(vec_eq(vec3(0.0f, 0.0f, 0.0f), placement.get_position()),
					"Position was incorrect after rotation");

		test_assert(
			vec_eq(vec3(0.0f, 0.0f, -1.0f), placement.get_right_direction()),
			"Right vector was incorrect after rotation");
		test_assert(
			vec_eq(vec3(0.0f, 1.0f, 0.0f), placement.get_up_direction()),
			"Up vector was incorrect after rotation");
		test_assert(
			vec_eq(vec3(1.0f, 0.0f, 0.0f), placement.get_forward_direction()),
			"Forward vector was incorrect after rotation");

		// Test translation

		placement.transform =
			glm::translate(vec3(-5.0f, 3.0f, 8.0f)) * placement.transform;

		test_assert(vec_eq(vec3(-5.0f, 3.0f, 8.0f), placement.get_position()),
					"Position was incorrect after translation");

		test_assert(
			vec_eq(vec3(0.0f, 0.0f, -1.0f), placement.get_right_direction()),
			"Right vector was incorrect after translation");
		test_assert(
			vec_eq(vec3(0.0f, 1.0f, 0.0f), placement.get_up_direction()),
			"Up vector was incorrect after translation");
		test_assert(
			vec_eq(vec3(1.0f, 0.0f, 0.0f), placement.get_forward_direction()),
			"Forward vector was incorrect after translation");
	}
}   // namespace glge::test

using namespace glge::test;

int main()
{
	Test(test_default).run();
	Test(test_move).run();
}

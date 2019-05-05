#include <glge/util/motion.h>

#include "test_utils.h"

namespace glge::test::cases
{
	/// <summary>
	/// Context for tests of motion utils.
	/// </summary>
	class MotionTest : public Test
	{
	private:
		util::Placement placement;

	public:
		/// \test Tests whether the default Placement's vectors and position are
		/// the expected values.
		void test_default()
		{
			test_assert(
				vec_eq(vec3(0.0f, 0.0f, 0.0f), placement.get_position()),
				"Default position was incorrect");

			test_assert(
				vec_eq(vec3(1.0f, 0.0f, 0.0f), placement.get_right_direction()),
				"Default right vector was incorrect");
			test_assert(
				vec_eq(vec3(0.0f, 1.0f, 0.0f), placement.get_up_direction()),
				"Default up vector was incorrect");
			test_assert(vec_eq(vec3(0.0f, 0.0f, 1.0f),
							   placement.get_forward_direction()),
						"Default forward vector was incorrect");
		}

		/// \test Tests rotating and translating an object, then verifying
		/// its orientation and position vectors are correct.
		void test_move()
		{
			// Test rotation

			auto rotation_matrix = glm::toMat4(
				glm::angleAxis(glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f)));

			placement.transform = rotation_matrix * placement.transform;

			test_assert(
				vec_eq(vec3(0.0f, 0.0f, 0.0f), placement.get_position()),
				"Position was incorrect after rotation");

			test_assert(vec_eq(vec3(0.0f, 0.0f, -1.0f),
							   placement.get_right_direction()),
						"Right vector was incorrect after rotation");
			test_assert(
				vec_eq(vec3(0.0f, 1.0f, 0.0f), placement.get_up_direction()),
				"Up vector was incorrect after rotation");
			test_assert(vec_eq(vec3(1.0f, 0.0f, 0.0f),
							   placement.get_forward_direction()),
						"Forward vector was incorrect after rotation");

			// Test translation

			placement.transform =
				glm::translate(vec3(-5.0f, 3.0f, 8.0f)) * placement.transform;

			test_assert(
				vec_eq(vec3(-5.0f, 3.0f, 8.0f), placement.get_position()),
				"Position was incorrect after translation");

			test_assert(vec_eq(vec3(0.0f, 0.0f, -1.0f),
							   placement.get_right_direction()),
						"Right vector was incorrect after translation");
			test_assert(
				vec_eq(vec3(0.0f, 1.0f, 0.0f), placement.get_up_direction()),
				"Up vector was incorrect after translation");
			test_assert(vec_eq(vec3(1.0f, 0.0f, 0.0f),
							   placement.get_forward_direction()),
						"Forward vector was incorrect after translation");
		}
	};
}   // namespace glge::test::cases


int main()
{
	using glge::test::Test;
	using namespace glge::test::cases;

	Test::run(&MotionTest::test_default);
	Test::run(&MotionTest::test_move);
}

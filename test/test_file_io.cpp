#include <internal/util/_util.h>

#include "test_utils.h"

namespace glge::test::cases
{
	/// \test Tests whether trying to open a nonexistent file throws an
	/// exception.
	void test_fail()
	{
		test_throws([] {
			std::ifstream stream =
				util::open_file_read("./never/a/real/file.nope");
		});

		test_throws([] {
			std::ofstream stream =
				util::open_file_write("./never/a/real/directory.nope");
		});

		test_throws([] {
			std::fstream stream =
				util::open_file_read_write("./never/a/real/directory.nope");
		});
	}
}   // namespace glge::test::cases


int main()
{
	using glge::test::Test;
	using namespace glge::test::cases;

    Test::run(test_fail);
}

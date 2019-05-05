#include <glge/support/proc_gen/l_system.h>

#include "test_utils.h"

namespace glge::test::cases
{
	using namespace glge::proc_gen;

	// Examples taken from:
	// https://en.wikipedia.org/wiki/L-system#Example_1:_Algae

	/// \test Tests that a system iterated 0 times returns the original
	/// input.
	void test_trivial()
	{
		const LString init = {'A'};
		const std::unordered_map<Variable, LString> rules{{'A', {'A', 'B'}},
														  {'B', {'A'}}};

		const LString result = d0L_system(init, rules, 0);

		const LString expected = {'A'};

		test_assert(expected == result, "Unexpected trivial result");
	}

	/// \test Tests that a system iterated once correctly applies the expected
	/// rule.
	void test_step()
	{
		const LString init = {'A'};
		const std::unordered_map<Variable, LString> rules{{'A', {'A', 'B'}},
														  {'B', {'A'}}};

		const LString result = d0L_system(init, rules, 1);

		const LString expected = {'A', 'B'};

		test_assert(expected == result, "Unexpected step result");
	}

	/// \test Tests that the system produces the expected result after a
	/// specified number of iterations.
	void test_algae()
	{
		const LString init = {'A'};
		const std::unordered_map<Variable, LString> rules{{'A', {'A', 'B'}},
														  {'B', {'A'}}};

		const LString result = d0L_system(init, rules, 5);

		const LString expected = {'A', 'B', 'A', 'A', 'B', 'A', 'B',
								  'A', 'A', 'B', 'A', 'A', 'B'};

		test_assert(expected == result, "Unexpected algae result");
	}
}   // namespace glge::test::cases


int main()
{
	using glge::test::Test;
	using namespace glge::test::cases;

    Test::run(test_trivial);
    Test::run(test_step);
    Test::run(test_algae);
}

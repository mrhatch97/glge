#include <glge/support/proc_gen/l_system.h>

#include "test_utils.h"

using namespace glge::proc_gen;

// Examples taken from: 
// https://en.wikipedia.org/wiki/L-system#Example_1:_Algae

void test_trivial()
{
	const LString init = {'A'};
	const std::unordered_map<Variable, LString> rules{{'A', {'A', 'B'}},
												{'B', {'A'}}};

	const LString result = d0L_system(init, rules, 0);

	const LString expected = {'A'};

	test_assert(expected == result, "Unexpected trivial result");
}

void test_step()
{
	const LString init = {'A'};
	const std::unordered_map<Variable, LString> rules{{'A', {'A', 'B'}},
												{'B', {'A'}}};

	const LString result = d0L_system(init, rules, 1);

	const LString expected = {'A', 'B'};

	test_assert(expected == result, "Unexpected step result");
}

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

int main()
{
  Test(test_trivial).run();
  Test(test_step).run();
	Test(test_algae).run();
}

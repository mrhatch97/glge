#include "glge/support/proc_gen/l_system.h"

#include <algorithm>

namespace glge::proc_gen
{
	LString
	d0L_system(const LString & start,
			   const std::unordered_map<Variable, LString> & production_rules,
			   unsigned int num_iterations)
	{
		LString cur_string = start;

		const auto end = cur_string.cend();
		const auto rule_end = production_rules.cend();

		// System iteration loop
		for (; num_iterations > 0; num_iterations--)
		{
			auto iter = cur_string.begin();

			// Loop over the characters of the string
			while (iter != end)
			{
				// Get current variable
				const Variable cur_var = *iter;

				// Look up variable in production rule list
				const auto rule_iter = production_rules.find(cur_var);

				// Is constant? Skip
				if (rule_iter == rule_end)
				{
					iter++;
					continue;
				}

				// Get produced string
				const LString & production = rule_iter->second;

				// Copy produced string into string
				std::copy(production.cbegin(), production.cend(),
						  std::inserter(cur_string, iter));

				// Remove the current variable from the string
				iter = cur_string.erase(iter);
			}
		}

		return cur_string;
	}
}   // namespace glge::proc_gen

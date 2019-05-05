/// <summary>Functions for procedural generation using L-systems.</summary>
///
/// \file l_system.h

#pragma once

#include <glge/common.h>

#include <list>
#include <unordered_map>

namespace glge::proc_gen
{
	/// <summary>Variable in an L-system grammar.</summary>
	using Variable = char;

	/// <summary>String of variables produced by an L-system.</summary>
	using LString = std::list<Variable>;

	/// <summary>
	/// Computes the result of a deterministic, context-free L-system iterated
	/// a given number of times.
	/// </summary>
	/// <param name="start">
	/// The initial string to iterate on. Also called the axiom or initiator.
	/// </param>
	/// <param name="production_rules">
	/// A map from variable to produced string. Variables not included in the
	/// map are assumed to be constants.
	/// </param>
	/// <param name="num_iterations">
	/// Number of iterations to run on the string.
	/// </param>
	/// <returns>Computed string of variables.</returns>
	LString
	d0L_system(const LString & start,
			   const std::unordered_map<Variable, LString> & production_rules,
			   unsigned int num_iterations);
}   // namespace glge::proc_gen

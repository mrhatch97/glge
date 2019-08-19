/// <summary>
///  Model file loading functions.
/// </summary>
///
/// \file obj_parser.h

#pragma once

#include <glge/model_parser/types.h>

namespace glge::model_parser
{
	/// <summary>
	/// Parse the given object file into an Object.
	/// </summary>
	/// <param name="filepath">
	/// Path to the object file to parse.
	/// </param>
	/// <returns>Parsed representation of the file contents.</returns>
	ModelData parse_object_file(czstring filepath);

}   // namespace glge::model_parser

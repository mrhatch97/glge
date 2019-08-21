/// <summary>
///  Model file loading functions.
/// </summary>
///
/// \file model_parser.h

#pragma once

#include <glge/model_parser/types.h>

namespace glge::model_parser
{
	/// <summary>
	/// Parse the given object file into a ModelData.
	/// </summary>
	/// <param name="filepath">
	/// Path to the object file to parse.
	/// </param>
	/// <returns>Parsed representation of the file contents.</returns>
	ModelData parse_object_file(czstring filepath);

	/// <summary>
	/// Write the given model data to a packed model file.
	/// </summary>
	/// <param name="filepath">
	/// Path to file to write to.
	/// </param>
	/// <param name="data">
	/// Data to write to file.
	/// </param>
	void write_packed_file(czstring filepath, const ModelData & data);

	/// <summary>
	/// Load the given packed file into a ModelData.
	/// </summary>
	/// <param name="filepath">
	/// Path to the packed file to load.
	/// </param>
	/// <returns>Representation of the file contents.</returns>
	ModelData read_packed_file(czstring filepath);

}   // namespace glge::model_parser

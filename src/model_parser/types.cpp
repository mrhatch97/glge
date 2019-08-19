#include "glge/model_parser/types.h"

#include <glge/model_parser/model_parser.h>

#include <filesystem>

namespace glge::model_parser
{
	ModelData ModelData::from_file(ModelFileInfo file_info)
	{
		switch (file_info.filetype)
		{
		case ModelFiletype::Object:
			return parse_object_file(file_info.filepath);

		case ModelFiletype::Auto:
		{
			auto path = std::filesystem::path(file_info.filepath);
			auto extension = path.extension();

			if (extension == ".obj")
			{
				return parse_object_file(file_info.filepath);
			}
			else
			{
				throw std::runtime_error(
					EXC_MSG("Auto-deduced model filetype was not supported"));
			}
		}
		default:
			throw std::logic_error(EXC_MSG("Unexpected model filetype"));
		}
	}
}   // namespace glge::model_parser

#include "glge/renderer/primitives/primitive_data.h"

#include <internal/util/compat.h>
#include <internal/util/util.h>

#include <algorithm>
#include <cstring>
#include <numeric>

namespace glge::renderer::primitive
{
	// TODO Improvements:
	// Read file into large buffer, parse chunk by chunk on multiple threads
	// instead of file IO every line
	ModelData ModelData::from_file(const ModelFileInfo & file_info)
	{
		Indices vertex_indices;
		Indices normal_indices;
		Indices uv_indices;
		VertexPoints vertices;
		NormalPoints normals;
		UVPoints uvs;

		std::FILE * file = util::try_fopen(file_info.filename.c_str(), "r");

		while (true)
		{
			char linebuffer[200];
			char tokenbuffer[50];
			char tokenbuffer2[2];
			char tokenchar[1];
			float fl[3];
			unsigned int in[9];
			bool indicesAllocated = false, verticesAllocated = false,
				 normalsAllocated = false, uvsAllocated = false;

			if (!std::fgets(linebuffer, sizeof(linebuffer), file))
			{
				break;
			}

			const char lineStart = linebuffer[0];
			const char lineSecond = linebuffer[1];

			switch (lineStart)
			{
			case 'v':
				switch (lineSecond)
				{
				case ' ':
					if (util::checked_sscanf(4, linebuffer, "%s %f %f %f",
											 FMT_STRING_ARG(tokenbuffer), fl,
											 fl + 1, fl + 2))
					{
						vertices.emplace_back(fl[0], fl[1], fl[2]);
					}
					break;
				case 'n':
					if (util::checked_sscanf(4, linebuffer, "%s %f %f %f",
											 FMT_STRING_ARG(tokenbuffer), fl,
											 fl + 1, fl + 2))
					{
						normals.emplace_back(fl[0], fl[1], fl[2]);
					}
					break;
				case 't':
					if (file_info.is_textured &&
						util::checked_sscanf(3, linebuffer, "%s %f %f",
											 FMT_STRING_ARG(tokenbuffer), fl,
											 fl + 1))
					{
						uvs.emplace_back(fl[0], fl[1]);
					}
					break;
				}
				break;
			case 'f':
				if (file_info.is_textured &&
					util::checked_sscanf(
						10, linebuffer, "%s %u/%u/%u %u/%u/%u %u/%u/%u",
						FMT_STRING_ARG(tokenbuffer), in, in + 1, in + 2, in + 3,
						in + 4, in + 5, in + 6, in + 7, in + 8))
				{
					for (int i = 0; i < 3; i++)
					{
						vertex_indices.push_back(in[i * 3 + 0] - 1);
						uv_indices.push_back(in[i * 3 + 1] - 1);
						normal_indices.push_back(in[i * 3 + 2] - 1);
					}
				}
				else if (util::checked_sscanf(
							 7, linebuffer, "%s %u//%u %u//%u %u//%u",
							 FMT_STRING_ARG(tokenbuffer), in, in + 1, in + 2,
							 in + 3, in + 4, in + 5))
				{
					for (int i = 0; i < 3; i++)
					{
						vertex_indices.push_back(in[i * 2 + 0] - 1);
						normal_indices.push_back(in[i * 2 + 1] - 1);
					}
				}
				break;
			case '#':
				if (!verticesAllocated || !indicesAllocated ||
					!normalsAllocated || !uvsAllocated)
				{
					if (util::checked_sscanf(3, linebuffer, "%c %s %u",
											 FMT_STRING_ARG(tokenbuffer2),
											 FMT_STRING_ARG(tokenbuffer), in))
					{
						if (!std::strcmp(tokenbuffer, "Vertices:"))
						{
							vertices.reserve(in[0]);
							normals.reserve(in[0]);
							verticesAllocated = true;
							uvsAllocated = true;
						}
						else if (!std::strcmp(tokenbuffer, "Faces:"))
						{
							vertex_indices.reserve(in[0]);
							normal_indices.reserve(in[0]);
							indicesAllocated = true;
						}
					}
					else if (util::checked_sscanf(4, linebuffer, "%c %s %c%u",
												  FMT_STRING_ARG(tokenbuffer2),
												  FMT_STRING_ARG(tokenbuffer),
												  FMT_STRING_ARG(tokenchar),
												  in))
					{
						if (tokenchar[0] != '[')
						{
							continue;
						}
						if (!std::strcmp(tokenbuffer, "vertices"))
						{
							vertices.reserve(in[0]);
							vertex_indices.reserve(in[0]);
							verticesAllocated = true;
						}
						else if (!std::strcmp(tokenbuffer, "normals"))
						{
							normals.reserve(in[0]);
							normal_indices.reserve(in[0]);
							normalsAllocated = true;
						}
						else if (!std::strcmp(tokenbuffer, "uvs"))
						{
							uvs.reserve(in[0]);
							uv_indices.reserve(in[0]);
							uvsAllocated = true;
						}
					}
				}
				break;
			default:
				continue;
			}
		}

		std::fclose(file);

		if constexpr (debug)
		{
			if (vertex_indices.size() == 0 || vertices.size() == 0)
			{
				throw std::logic_error(EXC_MSG("Loaded model had 0 vertices"));
			}
		}

		return ModelData{{vertices, vertex_indices},
						 {normals, normal_indices},
						 {uvs, uv_indices}};
	}

	EBOModelData ModelData::to_EBO_data(const ModelData & model_data)
	{
		return to_EBO_data(std::move(ModelData(model_data)));
	}

	EBOModelData ModelData::to_EBO_data(ModelData && model_data)
	{
		if (model_data.vertex_data.points.size() ==
				model_data.vertex_data.indices.size() &&
			model_data.normal_data.points.size() ==
				model_data.normal_data.indices.size() &&
			model_data.uv_data.points.size() ==
				model_data.uv_data.indices.size())
		{
			return EBOModelData{std::move(model_data.vertex_data.points),
								std::move(model_data.normal_data.points),
								std::move(model_data.uv_data.points),
								std::move(model_data.vertex_data.indices)};
		}

		auto copy_op = [=](auto & data) {
			auto copy = data.points;
			data.points.resize(data.indices.size());

			std::transform(
				EXECUTION_POLICY_SEQ data.indices.cbegin(), data.indices.cend(),
				data.points.begin(),
				[&](const typename decltype(data.indices)::size_type idx) {
					return copy.at(idx);
				});
		};

		try
		{
			copy_op(model_data.vertex_data);
			copy_op(model_data.normal_data);
			copy_op(model_data.uv_data);

			std::iota(model_data.vertex_data.indices.begin(),
					  model_data.vertex_data.indices.end(), 0);

			return EBOModelData{std::move(model_data.vertex_data.points),
								std::move(model_data.normal_data.points),
								std::move(model_data.uv_data.points),
								std::move(model_data.vertex_data.indices)};
		}
		catch (const std::exception &)
		{
			std::throw_with_nested(std::runtime_error(EXC_MSG(
				"Failed processing model info - model file may be invalid")));
		}
	}
}   // namespace glge::renderer::primitive

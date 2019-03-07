#pragma once

#include <glge/util/util.h>
#include <glge/util/compat.h>
#include <glge/common.h>
#include <glge/renderer/resources/resource_data.h>

#include <algorithm>
#include <cstring>
#include <iostream> // meh - why is a model loader printing stuff?
#include <numeric>

namespace glge::renderer::resource
{
		uptr<ModelData> load_model(const ModelFileInfo & fileinfo)
		{
			using namespace std::chrono;
			using clock = steady_clock;

			std::vector<unsigned int> vertex_indices;
			std::vector<unsigned int> normal_indices;
			std::vector<unsigned int> uv_indices;
			std::vector<vec3> input_vertices;
			std::vector<vec3> input_normals;
			std::vector<vec2> input_uvs;

			FILE * file;

			auto startTime = clock::now();
	#ifdef _WIN32
			if (fopen_s(&file, fileinfo.filename.c_str(), "r"))
	#else
			errno = 0;
			file = std::fopen(fileinfo.filename.c_str(), "r");

			if (errno)
	#endif
			{
				throw std::runtime_error(EXC_MSG("Failed to open model file " + fileinfo.filename));
			}

			while (true)
			{
				char linebuffer[200];
				char tokenbuffer[50];
				char tokenbuffer2[2];
				char tokenchar[1];
				float fl[3];
				unsigned int in[9];
				bool indicesAllocated = false, verticesAllocated = false, normalsAllocated = false, uvsAllocated = false;

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
						if (util::checked_fscanf(4, linebuffer, "%s %f %f %f", FMT_STRING_ARG(tokenbuffer), fl, fl + 1, fl + 2))
						{
							input_vertices.emplace_back(fl[0], fl[1], fl[2]);
						}
						break;
					case 'n':
						if (util::checked_fscanf(4, linebuffer, "%s %f %f %f", FMT_STRING_ARG(tokenbuffer), fl, fl + 1, fl + 2))
						{
							input_normals.emplace_back(fl[0], fl[1], fl[2]);
						}
						break;
					case 't':
						if (fileinfo.is_textured && util::checked_fscanf(3, linebuffer, "%s %f %f", FMT_STRING_ARG(tokenbuffer), fl, fl + 1))
						{
							input_uvs.emplace_back(fl[0], fl[1]);
						}
						break;
					}
					break;
				case 'f':
					if (fileinfo.is_textured &&
						util::checked_fscanf(10, linebuffer, "%s %u/%u/%u %u/%u/%u %u/%u/%u", FMT_STRING_ARG(tokenbuffer), in, in + 1, in + 2, in + 3, in + 4, in + 5, in + 6, in + 7, in + 8))
					{
						for (int i = 0; i < 3; i++)
						{
							vertex_indices.push_back(in[i * 3 + 0] - 1);
							uv_indices.push_back(in[i * 3 + 1] - 1);
							normal_indices.push_back(in[i * 3 + 2] - 1);
						}
					}
					else if (util::checked_fscanf(7, linebuffer, "%s %u//%u %u//%u %u//%u", FMT_STRING_ARG(tokenbuffer), in, in + 1, in + 2, in + 3, in + 4, in + 5))
					{
						for (int i = 0; i < 3; i++)
						{
							vertex_indices.push_back(in[i * 2 + 0] - 1);
							normal_indices.push_back(in[i * 2 + 1] - 1);
						}
					}
					break;
				case '#':
					if (!verticesAllocated || !indicesAllocated || !uvsAllocated)
					{
						if (util::checked_fscanf(3, linebuffer, "%c %s %u", FMT_STRING_ARG(tokenbuffer2), FMT_STRING_ARG(tokenbuffer), in))
						{
							if (!std::strcmp(tokenbuffer, "Vertices:"))
							{
								input_vertices.reserve(in[0]);
								input_normals.reserve(in[0]);
								verticesAllocated = true;
								normalsAllocated = true;
								uvsAllocated = true;
							}
							else if (!std::strcmp(tokenbuffer, "Faces:"))
							{
								vertex_indices.reserve(in[0]);
								normal_indices.reserve(in[0]);
								indicesAllocated = true;
							}
						}
						else if (util::checked_fscanf(4, linebuffer, "%c %s %c%u", FMT_STRING_ARG(tokenbuffer2), FMT_STRING_ARG(tokenbuffer), FMT_STRING_ARG(tokenchar), in))
						{
							if (tokenchar[0] != '[')
							{
								continue;
							}
							if (!std::strcmp(tokenbuffer, "vertices"))
							{
								input_vertices.reserve(in[0]);
								vertex_indices.reserve(in[0]);
								verticesAllocated = true;

							}
							else if (!std::strcmp(tokenbuffer, "normals"))
							{
								input_normals.reserve(in[0]);
								normal_indices.reserve(in[0]);
								normalsAllocated = true;
							}
							else if (!std::strcmp(tokenbuffer, "uvs"))
							{
								input_uvs.reserve(in[0]);
								uv_indices.reserve(in[0]);
								uvsAllocated = true;
							}
						}
					}
					break;
				case '\n':
				case 'o':
					continue;
				default:
					throw std::runtime_error(EXC_MSG("File had unrecognized line type"));
				}

			}

			fclose(file);

	#if _DEBUG
			if (vertex_indices.size() == 0 || input_vertices.size() == 0)
			{
				throw std::logic_error(EXC_MSG("Loaded model had 0 vertices"));
			}
	#endif

			Vector<vec3> vertices;
			Vector<vec3> normals;
			Vector<vec2> uvs;
			Vector<unsigned int> indices;

			vertices.resize(vertex_indices.size());
			normals.resize(normal_indices.size());
			uvs.resize(uv_indices.size());
			indices.resize(vertex_indices.size());

			try
			{
				std::transform(EXECUTION_POLICY_SEQ vertex_indices.cbegin(), vertex_indices.cend(), vertices.begin(),
					[&](const unsigned int idx) { return input_vertices.at(idx); });
				std::transform(EXECUTION_POLICY_SEQ normal_indices.cbegin(), normal_indices.cend(), normals.begin(),
					[&](const unsigned int idx) { return input_normals.at(idx); });
				std::transform(EXECUTION_POLICY_SEQ uv_indices.cbegin(), uv_indices.cend(), uvs.begin(),
					[&](const unsigned int idx) { return input_uvs.at(idx); });
				std::iota(indices.begin(), indices.end(), 0);
			}
			catch (...)
			{
				std::throw_with_nested(std::runtime_error(EXC_MSG("Failed processing model info - model file may be invalid")));
			}


			// meh
			auto completeTime = duration_cast<milliseconds>(clock::now() - startTime);

			{
				std::lock_guard<std::mutex> guard(stdio_write_lock);

				std::clog << "Loaded " << fileinfo.filename << " with "
					<< input_vertices.size() << " vertices, "
					<< input_normals.size() << " normals, "
					<< input_uvs.size() << " uvs, and "
					<< indices.size() << " indices in "
					<< completeTime.count() << " ms."
					<< std::endl;
			}

			return uptr<ModelData>(new ModelData{ vertices, normals, uvs, indices });
		}
}

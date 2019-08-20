#include "glge/model_parser/model_parser.h"

#include <internal/util/util.h>

#include <cstdint>

namespace glge::model_parser
{
	constexpr uint8_t current_file_version = 0;

	struct PackedModelHeader
	{
		std::uint8_t file_version;
		std::uint64_t vertex_count;
		std::uint64_t normal_count;
		std::uint64_t uv_count;
		std::uint64_t vertex_index_count;
		std::uint64_t normal_index_count;
		std::uint64_t uv_index_count;
	};

	template<typename T>
	size_t vector_size(const vector<T> & vec)
	{
		return vec.size() * sizeof(typename vector<T>::value_type);
	}

	template<typename T>
	const char * raw_data(const vector<T> & vec)
	{
		return reinterpret_cast<const char *>(vec.data());
	}

	template<typename T>
	void write_raw(std::ofstream & stream, const vector<T> & vec)
	{
		stream.write(raw_data(vec), vector_size(vec));
	}

	void write_packed_file(czstring filepath, const ModelData & data)
	{
		std::ofstream file = util::open_file_write(filepath, true, false, true);

		PackedModelHeader header{current_file_version,
								 data.vertex_data.points.size(),
								 data.normal_data.points.size(),
								 data.uv_data.points.size(),
								 data.vertex_data.indices.size(),
								 data.normal_data.indices.size(),
								 data.uv_data.indices.size()};

		file.write(reinterpret_cast<const char *>(&header), sizeof(header));

		write_raw(file, data.vertex_data.points);
		write_raw(file, data.normal_data.points);
		write_raw(file, data.uv_data.points);
		write_raw(file, data.vertex_data.indices);
		write_raw(file, data.normal_data.indices);
		write_raw(file, data.uv_data.indices);
	}

	template<typename T>
	char * raw_data(vector<T> & vec)
	{
		return reinterpret_cast<char *>(vec.data());
	}

	template<typename T>
	void read_raw(std::ifstream & stream,
				  vector<T> & vec,
				  std::uint64_t count)
	{
		vec.resize(count);
		stream.read(raw_data(vec),
					count * sizeof(typename vector<T>::value_type));
	}

	ModelData read_packed_file(czstring filepath)
	{
		std::ifstream file = util::open_file_read(filepath, true);

		PackedModelHeader header;

		file.read(reinterpret_cast<char *>(&header), sizeof(header));

		ModelData data;
		read_raw(file, data.vertex_data.points, header.vertex_count);
		read_raw(file, data.normal_data.points, header.normal_count);
		read_raw(file, data.uv_data.points, header.uv_count);
		read_raw(file, data.vertex_data.indices, header.vertex_index_count);
		read_raw(file, data.normal_data.indices, header.normal_index_count);
		read_raw(file, data.uv_data.indices, header.uv_index_count);

		return data;
	}
}   // namespace glge::model_parser

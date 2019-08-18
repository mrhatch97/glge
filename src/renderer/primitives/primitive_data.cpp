#include "glge/renderer/primitives/primitive_data.h"

#include <internal/util/compat.h>
#include <internal/util/util.h>

#include <algorithm>
#include <cstring>
#include <numeric>

namespace glge::renderer::primitive
{
	ModelData ModelData::from_file(const ModelFileInfo & file_info)
	{
		using namespace obj_parser;
		Object obj = parse_object_file(file_info.filename.c_str());

		return ModelData{obj.vertex_data, obj.normal_data, obj.uv_data};
	}

	EBOModelData ModelData::to_EBO_data(const ModelData & model_data)
	{
		return to_EBO_data(ModelData(model_data));
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

#include "glge/renderer/primitives/primitive_data.h"

#include <internal/util/compat.h>
#include <internal/util/util.h>

#include <algorithm>
#include <cstring>
#include <numeric>

namespace glge::renderer::primitive
{
	EBOModelData::EBOModelData(const ModelData & model_data) :
		EBOModelData(ModelData(model_data))
	{
	}

	EBOModelData::EBOModelData(ModelData && model_data)
	{
		auto move_op = [&model_data, this]() {
			this->vertices = std::move(model_data.vertex_data.points);
			this->normals = std::move(model_data.normal_data.points);
			this->uvs = std::move(model_data.uv_data.points);
			this->indices = std::move(model_data.vertex_data.indices);
		};

		if (model_data.vertex_data.points.size() ==
				model_data.vertex_data.indices.size() &&
			model_data.normal_data.points.size() ==
				model_data.normal_data.indices.size() &&
			model_data.uv_data.points.size() ==
				model_data.uv_data.indices.size())
		{
			move_op();
			return;
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
					  model_data.vertex_data.indices.end(),
					  model_parser::Index(0UL));

			move_op();
		}
		catch (const std::exception &)
		{
			std::throw_with_nested(std::runtime_error(EXC_MSG(
				"Failed processing model info - model file may be invalid")));
		}
	}
}   // namespace glge::renderer::primitive

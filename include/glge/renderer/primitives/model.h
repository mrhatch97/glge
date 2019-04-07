#pragma once

#include <glge/common.h>

#include <glge/renderer/primitives/renderable.h>
#include <glge/renderer/primitives/primitive_data.h>

namespace glge::renderer::primitive
{
	class Model : public Renderable
	{
	public:
		Model() = default;

		virtual ~Model() = default;

		static unique_ptr<Model> from_file(const ModelFileInfo & file_info);
		static unique_ptr<Model> from_data(const ModelData & model_data);
		static unique_ptr<Model> from_data(ModelData && model_data);
		static unique_ptr<Model> from_data(const EBOModelData & model_data);
	};
}

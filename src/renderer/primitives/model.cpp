#include "glge/primitives/model.h"

#include "gl_model.h"

namespace glge::renderer::primitive
{
	Model::Model(const res::ModelData & model_data) :
		model(std::make_unique<ModelImpl>(model_data))
	{
	}

	Model::~Model() = default;

	void Model::render() const
	{
		model->draw();
	}

	const res::ModelData & Model::get_data() const
	{
		return *model_data;
	}

	res::ModelData & Model::get_data()
	{
		return *model_data;
	}
}

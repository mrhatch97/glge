#include "glge/Primitives/Model.h"

#include "GLModel.h"

namespace eng::render::primitive
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
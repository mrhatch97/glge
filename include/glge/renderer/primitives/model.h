#pragma once

#include <glge/common.h>

#include <glge/renderer/primitives/renderable.h>
#include <glge/renderer/primitives/primitive_data.h>

namespace glge::renderer::primitive
{
	namespace opengl
	{
		class GLModel;
	}

	using ModelImpl = opengl::GLModel;

	class Model : public Renderable
	{
	private:
		uptr<ModelImpl> model;
	public:
		Model(const res::ModelData & model_data);

		~Model();

		void render() const override;
	};
}

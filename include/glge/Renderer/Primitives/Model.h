#pragma once

#include <glge/Common.h>

#include <glge/Renderer/Resources/ResourceData.h>
#include <glge/Renderer/Primitives/Renderable.h>

namespace eng::render::primitive
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
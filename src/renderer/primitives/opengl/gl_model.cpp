#include "gl_common.h"
#include "gl_buffer.h"

#include <glge/common.h>
#include <glge/renderer/primitives/model.h>

#include <array>

namespace glge::renderer::primitive
{
	namespace opengl
	{
		class GLModel : public Model
		{
		private:
			const GLsizei index_count;
			std::array<GLuint, 1> VAO;
			std::array<GLuint, 3> VBO;
			std::array<GLuint, 1> EBO;
			bool destroy;

		public:

			GLModel(const GLModel &) = delete;

			GLModel(GLModel && other) : 
				index_count(other.index_count),
				VAO(other.VAO),
				VBO(other.VBO),
				EBO(other.EBO),
				destroy(other.destroy)
			{
				other.destroy = false;
			}

			GLModel(const EBOModelData & model_data) :
				index_count(static_cast<GLsizei>(model_data.indices.size())), 
        destroy(false)
			{
				glGenVertexArrays(static_cast<GLsizei>(VAO.size()), VAO.data());
				glGenBuffers(static_cast<GLsizei>(VBO.size()), VBO.data());
				glGenBuffers(static_cast<GLsizei>(EBO.size()), EBO.data());

				renderer::opengl::throw_if_gl_error(
            EXC_MSG("Failed to generate requisite storage for model"));

				{
					util::UniqueHandle vaoBind(
              [&] { glBindVertexArray(VAO[0]); }, [] { glBindVertexArray(0); });

					bind_attrib_data(VBO[vertex_index], vertex_index, 
              model_data.vertices, false);

					if (!model_data.normals.empty())
					{
						bind_attrib_data(VBO[normal_index], normal_index, 
                model_data.normals, true);
					}
					if (!model_data.uvs.empty())
					{
						bind_attrib_data(VBO[texcor_index], texcor_index, 
                model_data.uvs, false);
					}

					bind_element_array(EBO[0], model_data.indices);
				}

				destroy = true;
			}

			~GLModel()
			{
				if (destroy)
				{
					glDeleteVertexArrays(static_cast<GLsizei>(VAO.size()), VAO.data());
					glDeleteBuffers(static_cast<GLsizei>(VBO.size()), VBO.data());
					glDeleteBuffers(static_cast<GLsizei>(EBO.size()), EBO.data());
				}
			}

			GLModel & operator=(const GLModel &) = delete;

			GLModel & operator=(GLModel && other) = delete;

			void render() const override
			{
				util::UniqueHandle vaoBind(
					[&] { glBindVertexArray(VAO[0]); },
					[] { glBindVertexArray(0); }
				);

				if constexpr (debug)
				{
					renderer::opengl::throw_if_gl_error(EXC_MSG("Error prior to render"));
				}

				glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);

				if constexpr (debug)
				{
					renderer::opengl::throw_if_gl_error(EXC_MSG("Error during render"));
				}
			}

			GLuint getVAO() const { return VAO[0]; }
		};
	}

	unique_ptr<Model> Model::from_file(const ModelFileInfo & file_info)
	{
		ModelData model_data = ModelData::from_file(file_info);
		return Model::from_data(std::move(model_data));
	}

	unique_ptr<Model> Model::from_data(const ModelData & model_data)
	{
		EBOModelData ebo_data = ModelData::to_EBO_data(model_data);
		return Model::from_data(ebo_data);
	}

	unique_ptr<Model> Model::from_data(ModelData && model_data)
	{
		EBOModelData ebo_data = ModelData::to_EBO_data(std::forward<ModelData>(model_data));
		return Model::from_data(ebo_data);
	}

	unique_ptr<Model> Model::from_data(const EBOModelData & ebo_data)
	{
		return std::make_unique<opengl::GLModel>(ebo_data);
	}
}

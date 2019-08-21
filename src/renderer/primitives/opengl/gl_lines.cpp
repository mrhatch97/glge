#include "gl_buffer.h"
#include "gl_common.h"

#include <glge/common.h>
#include <glge/renderer/primitives/lines.h>
#include <internal/util/_util.h>

#include <array>

namespace glge::renderer::primitive
{
	namespace opengl
	{
		class GLLines : public Lines
		{
		private:
			const GLsizei vertex_count;
			std::array<GLuint, 1> VAO;
			std::array<GLuint, 1> VBO;
			bool destroy;

		public:
			GLLines(const vector<vec3> & points) :
				vertex_count(util::safe_cast<size_t, GLsizei>(points.size())),
				destroy(false)
			{
				glGenVertexArrays(static_cast<GLsizei>(VAO.size()), VAO.data());
				glGenBuffers(static_cast<GLsizei>(VBO.size()), VBO.data());

				renderer::opengl::throw_if_gl_error(
					EXC_MSG("Failed to generate requisite storage for lines"));

				destroy = true;

				{
					util::UniqueHandle vaoBind(
						[&] { glBindVertexArray(VAO[0]); },
						[] { glBindVertexArray(0); });

					bind_attrib_data(VBO[vertex_index], vertex_index, points,
									 false);
				}
			}

			GLLines(const GLLines &) = delete;

			GLLines(GLLines && other) :
				vertex_count(other.vertex_count), VAO(other.VAO),
				VBO(other.VBO), destroy(other.destroy)
			{
				other.destroy = false;
			}

			~GLLines()
			{
				if (destroy)
				{
					glDeleteVertexArrays(
						util::safe_cast<typename decltype(VAO)::size_type,
										GLsizei>(VAO.size()),
						VAO.data());
					glDeleteBuffers(
						util::safe_cast<typename decltype(VBO)::size_type,
										GLsizei>(VBO.size()),
						VBO.data());
				}
			}

			GLLines & operator=(const GLLines &) = delete;
			GLLines & operator=(GLLines &&) = delete;

			void render() const override
			{
				util::UniqueHandle vaoBind([&] { glBindVertexArray(VAO[0]); },
										   [] { glBindVertexArray(0); });

				if constexpr (debug)
				{
					renderer::opengl::throw_if_gl_error(
						EXC_MSG("Error prior to render"));
				}

				glDrawArrays(GL_LINE_LOOP, 0, vertex_count);

				if constexpr (debug)
				{
					renderer::opengl::throw_if_gl_error(
						EXC_MSG("Error during render"));
				}
			}
		};
	}   // namespace opengl

	unique_ptr<Lines> Lines::from_points(const vector<vec3> & points)
	{
		return std::make_unique<opengl::GLLines>(points);
	}
}   // namespace glge::renderer::primitive

#include <SOIL.h>

#include <glge/common.h>
#include <glge/renderer/primitives/cubemap.h>
#include <glge/util/util.h>

#include "gl_common.h"

namespace glge::renderer::primitive
{
	namespace opengl
	{
		class GLCubemap : public Cubemap
		{
		private:
			const GLuint id;
			bool destroy;

		public:
			GLCubemap(const CubemapFileInfo & info) :
				id(SOIL_load_OGL_cubemap(info.right.path.c_str(),
										 info.left.path.c_str(),
										 info.top.path.c_str(),
										 info.bottom.path.c_str(),
										 info.back.path.c_str(),
										 info.front.path.c_str(),
										 SOIL_LOAD_RGB,
										 SOIL_CREATE_NEW_ID,
										 SOIL_FLAG_MIPMAPS)),
				destroy(true)
			{}

			GLCubemap(const GLCubemap &) = delete;

			GLCubemap(GLCubemap && other) : id(other.id), destroy(other.destroy)
			{
				other.destroy = false;
			}

			GLCubemap & operator=(const GLCubemap &) = delete;

			GLCubemap & operator=(GLCubemap && other) = delete;

			void activate() const override
			{
				glBindTexture(GL_TEXTURE_CUBE_MAP, id);
			}

			~GLCubemap()
			{
				if (destroy)
				{
					glDeleteTextures(1, &id);
				}
			}
		};
	}   // namespace opengl

	unique_ptr<Cubemap> Cubemap::from_file(const CubemapFileInfo & file_info)
	{
		return std::make_unique<opengl::GLCubemap>(file_info);
	}
}   // namespace glge::renderer::primitive

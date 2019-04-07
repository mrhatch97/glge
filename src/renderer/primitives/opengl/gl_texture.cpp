#include "gl_common.h"

#include <glge/common.h>
#include <glge/renderer/primitives/texture.h>

#include <SOIL.h>

namespace glge::renderer::primitive
{
	namespace opengl
	{
		class GLTexture : public Texture
		{
		private:
			const GLuint id;
			bool destroy;

			static GLuint load_texture_file(const TextureFileInfo & info)
			{
				GLuint texture = SOIL_load_OGL_texture(info.path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
					SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_TEXTURE_REPEATS);

				if (!texture)
				{
					czstring soil_err = SOIL_last_result();
					throw std::runtime_error(EXC_MSG("Failed to load texture file: " + info.path + "\nSOIL error: " + std::string(soil_err)));
				}

				return texture;
			}

		public:
			void activate() const override
			{
				glBindTexture(GL_TEXTURE_2D, id);
			}

			GLuint getId() { return id; }

			GLTexture(const TextureFileInfo & info) : id(load_texture_file(info)), destroy(true)
			{
			}

			GLTexture(const GLTexture &) = delete;
			GLTexture(GLTexture && other) :
					id(other.id), destroy(other.destroy)
			{
					other.destroy = false;
			}

			GLTexture & operator=(const GLTexture &) = delete;
			GLTexture & operator=(GLTexture &&) = delete;

			~GLTexture()
			{
				if (destroy)
				{
					glDeleteTextures(1, &id);
				}
			}
		};
	}

	unique_ptr<Texture> Texture::from_file(const TextureFileInfo & file_info)
	{
		return std::make_unique<opengl::GLTexture>(file_info);
	}
}

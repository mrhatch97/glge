#include <glge/renderer/primitives/shader_program.h>
#include <glge/renderer/primitives/texture.h>
#include <glge/renderer/render_settings.h>
#include <glge/renderer/renderer.h>
#include <glge/util/motion.h>

#include "ogl_test_utils.h"


namespace glge::test::opengl::cases
{
	using namespace glge::renderer;
	using namespace glge::renderer::primitive;

    /// <summary>
    /// Context for Shader parameterization tests.
    /// </summary>
	class ShaderParameterizationTest : public OGLTest
	{
	public:
        /// \test Tests whether a Shader can have its parameters loaded into
        /// VRAM by OpenGL after load.
		void test_load()
		{
			auto texture_shader = TextureShader::load();
			auto tex_ptr = Texture::from_file(
				TextureFileInfo{"./resources/textures/test.png"});
			auto texture_instance = texture_shader->instance(*tex_ptr);

			RenderSettings settings{std::make_unique<Camera>(
				CameraIntrinsics{}, util::Placement{})};
			RenderParameters params(settings, mat4());

			{
				auto shader_bind = texture_shader->bind();

				texture_instance(params);
			}
		}
	};
}   // namespace glge::test::opengl::cases


int main()
{
    using glge::test::Test;
    using glge::test::opengl::cases::ShaderParameterizationTest;
    Test::run(&ShaderParameterizationTest::test_load);
}

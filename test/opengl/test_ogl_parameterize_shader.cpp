#include <glge/renderer/primitives/shader_program.h>
#include <glge/renderer/primitives/texture.h>
#include <glge/renderer/render_settings.h>
#include <glge/renderer/renderer.h>
#include <glge/util/motion.h>

#include "ogl_test_utils.h"

using namespace glge;
using namespace glge::renderer;
using namespace glge::renderer::primitive;

void test_load()
{
	auto texture_shader = TextureShader::load();
	auto tex_ptr =
		Texture::from_file(TextureFileInfo{"./resources/textures/test.png"});
	auto texture_instance = texture_shader->instance(*tex_ptr);

	RenderSettings settings{
		std::make_unique<Camera>(CameraIntrinsics{}, util::Placement{})};
	RenderParameters params(settings, mat4());

	{
		auto shader_bind = texture_shader->bind();

		texture_instance(params);
	}
}

int main()
{
	OGLTest(test_load).run();
}
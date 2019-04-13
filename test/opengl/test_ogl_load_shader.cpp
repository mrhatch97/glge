#include <glge/renderer/primitives/shader_program.h>
#include <glge/renderer/primitives/texture.h>
#include <glge/renderer/renderer.h>

#include "ogl_test_utils.h"

using namespace glge;
using namespace glge::renderer::primitive;

void test_load()
{
	auto normal_shader = NormalShader::load();
	auto color_shader = ColorShader::load();
	auto texture_shader = TextureShader::load();
	auto skybox_shader = SkyboxShader::load();
	auto envmap_shader = EnvMapShader::load();
}

void test_instance()
{
	auto normal_shader = NormalShader::load();
	auto color_shader = ColorShader::load();
	auto texture_shader = TextureShader::load();
	auto skybox_shader = SkyboxShader::load();
	auto envmap_shader = EnvMapShader::load();

	auto tex_ptr =
		Texture::from_file(TextureFileInfo{"./resources/textures/test.png"});
	auto cubemap_ptr = Cubemap::from_file(CubemapFileInfo{
		"./resources/cubemaps/test_up.tga", "./resources/cubemaps/test_dn.tga",
		"./resources/cubemaps/test_lf.tga", "./resources/cubemaps/test_rt.tga",
		"./resources/cubemaps/test_ft.tga",
		"./resources/cubemaps/test_bk.tga"});

	auto normal_instance = normal_shader->instance();
	auto color_instance = color_shader->instance(vec3(1.0f, 1.0f, 1.0f));
	auto texture_instance = texture_shader->instance(*tex_ptr);
	auto skybox_instance = skybox_shader->instance(*cubemap_ptr);
	auto envmap_instance = envmap_shader->instance(*cubemap_ptr);
}

int main()
{
	OGLTest(test_load).run();
	OGLTest(test_instance).run();
}
#include "gl_common.h"
#include "glge/renderer/renderer.h"

namespace glge::renderer
{
	namespace opengl
	{
		inline void setup_glew()
		{
			using namespace std::string_literals;

			// Initialize GLEW. Not needed on OSX systems.
			GLenum err = glewInit();
			if (err != GLEW_OK)
			{
				std::string err_msg(
					reinterpret_cast<czstring>(glewGetErrorString(err)));
				/* Problem: glewInit failed, something is seriously wrong. */
				throw std::runtime_error(
					EXC_MSG("Failed to initialize GLEW: "s + err_msg));
			}
			// printf("Current GLEW version: %s\n",
			// glewGetString(GLEW_VERSION));
		}

		inline void print_versions()
		{
			// Get info of GPU and supported OpenGL version
			// printf("Renderer: %s\n", glGetString(GL_RENDERER));
			// printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

			// If the shading language symbol is defined
#ifdef GL_SHADING_LANGUAGE_VERSION
			// std::printf("Supported GLSL version is %s.\n",
			// glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
		}

		inline void setup_opengl_settings()
		{
#if !GLGE_APPLE
			// Setup GLEW. Don't do this on OSX systems.
			setup_glew();
#endif
			// Enable depth buffering
			glEnable(GL_DEPTH_TEST);
			// Related to shaders and z value comparisons for the depth buffer
			glDepthFunc(GL_LEQUAL);
			// Set polygon drawing mode to fill front and back of each polygon
			// You can also use the paramter of GL_LINE instead of GL_FILL to
			// see wireframes
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			// Disable backface culling to render both sides of polygons
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

			// Set clear color
			glClearColor(0.05f, 0.8f, 0.85f, 1.0f);
		}
	}   // namespace opengl

	void configure_environment() { opengl::setup_opengl_settings(); }
}   // namespace glge::renderer

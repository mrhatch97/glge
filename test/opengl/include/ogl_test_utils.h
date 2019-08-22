/// <summary>
/// Utilities and helpers for running unit tests on code
/// using OpenGL.
/// </summary>
/// \file ogl_test_utils.h

#pragma once

#include <glge/renderer/renderer.h>
#include <glge/util/util.h>

#include "test_utils.h"

#ifdef GLGE_HEADLESS_TESTS
#include <EGL/egl.h>
#else
#include <GLFW/glfw3.h>
#endif

namespace glge::test::opengl
{

#ifdef GLGE_HEADLESS_TESTS
	// Taken from
	// https://devblogs.nvidia.com/egl-eye-opengl-visualization-without-x-server/

	static const EGLint config_attribs[] = {EGL_SURFACE_TYPE,
											EGL_PBUFFER_BIT,
											EGL_BLUE_SIZE,
											8,
											EGL_GREEN_SIZE,
											8,
											EGL_RED_SIZE,
											8,
											EGL_DEPTH_SIZE,
											8,
											EGL_RENDERABLE_TYPE,
											EGL_OPENGL_BIT,
											EGL_NONE};

	static const int pbuffer_width = 9;
	static const int pbuffer_height = 9;

	static const EGLint pbuffer_attribs[] = {
		EGL_WIDTH, pbuffer_width, EGL_HEIGHT, pbuffer_height, EGL_NONE,
	};

	/// <summary>
	/// Creates an OpenGL context for a test to run under.
	/// </summary>
	/// <returns>UniqueHandle managing the disposal of the context.</returns>
	glge::util::UniqueHandle create_ogl_context()
	{
		// TODO this won't work - display will be freed at the end of this
		// method
		EGLDisplay display;
		return glge::util::UniqueHandle(
			[&] {
				display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
				if (display == EGL_NO_DISPLAY)
				{
					throw std::runtime_error("Could not initialize EGL");
				}

				EGLint major, minor;

				if (!eglInitialize(display, &major, &minor))
				{
					throw std::runtime_error(
						"Could not start EGL display connection");
				}

				EGLint num_configs;
				EGLConfig config;

				if (!eglChooseConfig(display, nullptr, &config, 1,
									 &num_configs))
				{
					throw std::runtime_error("Configuration selection failed");
				}

				EGLSurface surface =
					eglCreatePbufferSurface(display, config, pbuffer_attribs);

				if (!eglBindAPI(EGL_OPENGL_API))
				{
					throw std::runtime_error("API bind failed");
				}

				EGLContext context =
					eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);

				if (eglMakeCurrent(display, surface, surface, context) !=
					EGL_TRUE)
				{
					throw std::runtime_error("Display was not made current");
				}
			},
			[&] { eglTerminate(display); });
	}

#else
	/// <summary>
	/// Creates an OpenGL context for a test to run under.
	/// </summary>
	/// <returns>UniqueHandle managing the disposal of the context.</returns>
	glge::util::UniqueHandle create_ogl_context()
	{
		return glge::util::UniqueHandle(
			[] {
				if (!glfwInit())
				{
					throw std::runtime_error("Failed to initialize GLFW");
				}

				auto glfwWindow =
					glfwCreateWindow(100, 100, "Test", NULL, NULL);

				if (!glfwWindow)
				{
					glfwTerminate();
					throw std::runtime_error("Failed to create GLFW window");
				}

				glfwMakeContextCurrent(glfwWindow);
			},
			[] { glfwTerminate(); });
	}
#endif

	/// <summary>
	/// Context for tests using OpenGL.
	/// </summary>
	class OGLTest : public Test
	{
	private:
		glge::util::UniqueHandle opengl_context;

	protected:
		/// <summary>
		/// Function to run before the main test.
		/// </summary>
		void pre_test() override
		{
			opengl_context = create_ogl_context();

			glge::renderer::configure_environment();
		};

		/// <summary>
		/// Function to run after the main test.
		/// </summary>
		void post_test() override { opengl_context.reset(); };

	public:
		/// <summary>Default destructor.</summary>
		virtual ~OGLTest() = default;
	};
}   // namespace glge::test::opengl

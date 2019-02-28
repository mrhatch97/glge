#pragma once

#include <glge/Util/Compat.h>

#if GLGE_APPLE
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#define OGL_ERROR_CHK(ERROR_ACTION) if (GLenum err; (err = glGetError()) != GL_NO_ERROR) { ERROR_ACTION }

#if _DEBUG
#define DBG_OGL_ERROR_CHK(ERROR_ACTION) OGL_ERROR_CHK(ERROR_ACTION)
#else
#define DBG_OGL_ERROR_CHK(ERROR_ACTION)
#endif

void setup_opengl_settings();
void setup_glew();
void print_versions();

constexpr GLuint GL_NO_PROGRAM = 0;
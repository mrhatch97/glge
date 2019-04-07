message("-- glge backend is OpenGL")

# Based on https://stackoverflow.com/a/47801116
# Allows including the content of arbitrary files in a
# C++ string at compile time
function(make_includable infile outfile)
	file(READ ${infile} content)
	set(delim "TEXT_INCLUDE")
	set(content "R\"${delim}(\n${content})${delim}\"")
	file(WRITE ${outfile} "${content}")
endfunction(make_includable)

target_sources(glge
	PRIVATE
		gl_config.cpp
		gl_program.h
		../primitives/opengl/gl_cubemap.cpp
		../primitives/opengl/gl_lines.cpp
		../primitives/opengl/gl_model.cpp
		../primitives/opengl/gl_texture.cpp
		../primitives/opengl/gl_shader.cpp
)

target_include_directories(glge
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}
)

if(WIN32)
	find_package(OpenGL REQUIRED)
	find_package(GLEW REQUIRED)

	target_link_libraries(glge
		PRIVATE
			OpenGL::GL
			GLEW::GLEW
	)

elseif(UNIX)
	find_package(OpenGL REQUIRED)

	if(OpenGL_GLX_FOUND AND OpenGL_EGL_FOUND)
			message("-- glge building with GLVND - EGL and GLX supported")
			target_link_libraries(glge
					PRIVATE
							OpenGL::OpenGL
							OpenGL::EGL
							OpenGL::GLX
			)

	elseif(OpenGL_GLX_FOUND)
			message("-- glge building with GLVND - GLX supported")
			target_link_libraries(glge
					PRIVATE
							OpenGL::OpenGL
							OpenGL::GLX
			)

	else()
			message("-- glge building with legacy OpenGL")
			target_link_libraries(glge
					PRIVATE
							OpenGL::GL
			)

	endif()

	find_package(GLEW REQUIRED)
	target_link_libraries(glge
		PRIVATE
				GLEW::GLEW
	)
elseif(APPLE)
	find_package(OpenGL REQUIRED)

	find_library(COCOA Cocoa)
    if(NOT COCOA)
            message(FATAL_ERROR "Cocoa not found")
    endif()

    find_library(IOKIT IOKit)
    if(NOT IOKIT)
            message(FATAL_ERROR "IOKit not found")
    endif()

    find_library(COREVIDEO CoreVideo)
    if(NOT COREVIDEO)
            message(FATAL_ERROR "CoreVideo not found")
    endif()

	target_link_libraries(glge
		PRIVATE
			OpenGL::GL
			${COCOA}
			${IOKIT}
			${COREVIDEO}
	)
endif()

find_package(SOIL REQUIRED)

target_link_libraries(glge
	PRIVATE
		SOIL::SOIL
)

# Generate delimited GLSL code for inclusion in C++ at compile-time
file(GLOB GLSL_SOURCES
	RELATIVE ${PROJECT_SOURCE_DIR}/src/renderer/primitives/opengl
${PROJECT_SOURCE_DIR}/src/renderer/primitives/opengl/glsl/*.glsl)

foreach(_file ${GLSL_SOURCES})
	make_includable(${PROJECT_SOURCE_DIR}/src/renderer/primitives/opengl/${_file} ${PROJECT_BINARY_DIR}/generated/include/generated/${_file})
endforeach(_file)

target_include_directories(glge
	PRIVATE
		${PROJECT_BINARY_DIR}/generated/include
)

# Based on https://stackoverflow.com/a/47801116
# Allows including the content of arbitrary files in a
# C++ string at compile time
function(make_includable infile outfile)
	file(READ ${infile} content)
	set(delim "TEXT_INCLUDE")
	set(content "R\"${delim}(\n${content})${delim}\"")
	file(WRITE ${outfile} "${content}")
endfunction(make_includable)

target_include_directories(glge
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
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

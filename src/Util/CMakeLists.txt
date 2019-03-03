add_library(util OBJECT
	math.cpp
	Motion.cpp
	Util.cpp
)

target_include_directories(util
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

target_compile_features(util
	PUBLIC
		cxx_std_17
)

target_compile_definitions(util
	PRIVATE
		$<$<CONFIG:Debug>:_DEBUG>
		$<$<CONFIG:RelWithDebInfo>:_DEBUG>
		$<$<CONFIG:Release>:NDEBUG>
		$<$<CONFIG:MinSizeRel>:NDEBUG>
		GLM_FORCE_SILENT_WARNINGS
)

target_link_libraries(util
	PUBLIC
		glm
)

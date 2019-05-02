target_sources(glge
	PRIVATE
		event/input/input_dispatcher.cpp
		proc_gen/terrain.cpp
    proc_gen/l_system.cpp
)

target_include_directories(glge
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/proc_gen
)

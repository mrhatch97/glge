target_sources(glge
	PRIVATE
		types.cpp
		obj_parser.cpp
		packed_parser.cpp
)

target_include_directories(glge
	PRIVATE
		model_parser)

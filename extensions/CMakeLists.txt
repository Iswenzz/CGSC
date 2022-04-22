add_subdirectory(__test__)
add_subdirectory(versions)

target_sources(CGSC PRIVATE
	functions.c
	functions.h
	variables.c
	variables.h)

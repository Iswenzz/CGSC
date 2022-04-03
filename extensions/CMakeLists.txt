add_subdirectory(__test__)
add_subdirectory(versions)

target_sources(CGSC PRIVATE
	cgsc_param.c
	cgsc_param.h
	cgsc_variable.c
	cgsc_variable.h)

add_subdirectory(sys)
add_subdirectory(utils)
add_subdirectory(versions)

target_sources(CGSC PRIVATE
   cgsc_cod4x.h
   cgsc_plugin.h)

add_subdirectory(sys)
add_subdirectory(utils)
add_subdirectory(versions)

target_sources(CGSC PRIVATE
   cod4x.h
   plugin.h)

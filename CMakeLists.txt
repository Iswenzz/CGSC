cmake_minimum_required(VERSION 3.16.0)
project(CGSC VERSION 1.1.0 LANGUAGES C)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "-m32 -Wall -msse2 -mfpmath=sse -fno-omit-frame-pointer -fno-pie -O1")

# NASM
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(CMAKE_ASM_NASM_FLAGS "-dWin32 --prefix _")
	set(CMAKE_ASM_NASM_OBJECT_FORMAT "win32")
else()
	set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf32")
endif()
enable_language(ASM_NASM)

# CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR 7Z)
include(CPack)

# Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Tests
if(ENABLE_TESTING)
	include(CTest)
	add_executable(CGSC.Tests)
	target_link_libraries(CGSC.Tests PUBLIC ${CONAN_LIBS} CGSC)
	add_test(NAME Tests COMMAND CGSC.Tests)
endif()

# Build
add_library(CGSC)
add_subdirectory(asm)
add_subdirectory(extensions)
add_subdirectory(sys)
add_subdirectory(utils)
add_subdirectory(versions)

target_compile_definitions(CGSC PUBLIC COD4X18UPDATE)
target_include_directories(CGSC PUBLIC . .. ../version ../../plugins)
target_sources(CGSC PRIVATE cgsc.h)

set_target_properties(CGSC PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../../../lib)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

# Pack
install(TARGETS CGSC LIBRARY DESTINATION lib)
install(DIRECTORY src/ DESTINATION lib/include/CGSC
    FILES_MATCHING PATTERN "*.h" PATTERN "__test__" EXCLUDE)

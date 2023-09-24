include(.vcpkg/cmake/env.cmake)
set(VCPKG_TARGET_TRIPLET x86-mingw-static)

option(BUILD_TESTING "Build tests" OFF)
if(BUILD_TESTING)
	list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

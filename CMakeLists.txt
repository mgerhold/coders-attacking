cmake_minimum_required(VERSION 3.28)

if (WIN32)
    set(VCPKG_TARGET_TRIPLET x64-windows-static)
endif ()

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)

project(coders_attacking C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include(${PROJECT_SOURCE_DIR}/project_options.cmake)
include(${PROJECT_SOURCE_DIR}/dependencies.cmake)

coders_attacking_setup_dependencies()

add_subdirectory(src bin)
add_subdirectory(vendor)

if (${coders_attacking_build_tests})
    enable_testing()
    add_subdirectory(test)
endif ()


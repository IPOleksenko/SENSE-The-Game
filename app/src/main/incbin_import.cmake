include(FetchContent)

FetchContent_Declare(
    incbin_upstream
    GIT_REPOSITORY https://github.com/graphitemaster/incbin.git
    GIT_TAG main
    SOURCE_DIR ${PROJECT_ROOT}/external/incbin
)

FetchContent_MakeAvailable(incbin_upstream)

set(_src_incbin_h "${incbin_upstream_SOURCE_DIR}/incbin.h")
set(_dst_include_dir "${incbin_upstream_SOURCE_DIR}/include")
set(_dst_incbin_h "${_dst_include_dir}/incbin.h")
set(_incbin_cmake "${incbin_upstream_SOURCE_DIR}/CMakeLists.txt")

if(EXISTS ${_src_incbin_h} AND NOT EXISTS ${_dst_incbin_h})
    file(MAKE_DIRECTORY ${_dst_include_dir})
    file(RENAME ${_src_incbin_h} ${_dst_incbin_h})
endif()

if(NOT EXISTS ${_incbin_cmake})
    file(WRITE ${_incbin_cmake} "
cmake_minimum_required(VERSION 3.21)

project(incbin LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(incbin INTERFACE)

target_include_directories(
    incbin INTERFACE
    \${CMAKE_CURRENT_LIST_DIR}/include
)

add_executable(incbin_tool \${CMAKE_CURRENT_LIST_DIR}/incbin.c)
")
endif()

if(NOT TARGET incbin)
    add_subdirectory(${incbin_upstream_SOURCE_DIR} ${CMAKE_BINARY_DIR}/incbin_build)
endif()

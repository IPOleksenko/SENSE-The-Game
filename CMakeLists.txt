cmake_minimum_required(VERSION 3.21)
project(SENSE_THE_GAME LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(sdl2_import.cmake)
include(incbin_import.cmake)

set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(ASSETS_DIR ${PROJECT_ROOT}/assets)
set(SOURCE_DIR ${PROJECT_ROOT}/src)

include(${SOURCE_DIR}/window/module.cmake)
include(${SOURCE_DIR}/utils/module.cmake)
include(${SOURCE_DIR}/objects/module.cmake)
include(${SOURCE_DIR}/events/module.cmake)
include(${SOURCE_DIR}/assets/module.cmake)

set(SRC_FILES
    src/main.cpp
)

# Specify custom output directories
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})


# Create the main executable
add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    src/main.cpp
)

# Add dependencies and link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL2::SDL2main
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf

        ${PROJECT_NAME}_window
        ${PROJECT_NAME}_utils
        ${PROJECT_NAME}_objects
        ${PROJECT_NAME}_events
        ${PROJECT_NAME}_assets
    )

if(MSVC)
    include(msvc_config.cmake)
endif()

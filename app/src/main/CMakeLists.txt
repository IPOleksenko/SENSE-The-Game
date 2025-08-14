cmake_minimum_required(VERSION 3.21)
project(SENSE_THE_GAME LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cpp)

set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(ASSETS_DIR ${PROJECT_ROOT}/assets)
set(SOURCE_DIR ${PROJECT_ROOT}/cpp)

include(sdl2_import.cmake)
include(incbin_import.cmake)


include(${SOURCE_DIR}/assets/module.cmake)
include(${SOURCE_DIR}/utils/module.cmake)
include(${SOURCE_DIR}/objects/module.cmake)
include(${SOURCE_DIR}/application/module.cmake)

set(SRC_FILES
    ${SOURCE_DIR}/main.cpp
)

if(ANDROID)
    include(android_config.cmake)

    add_library(${PROJECT_NAME} SHARED
        ${SRC_FILES}
    )
else()
    add_executable(${PROJECT_NAME}
        ${SRC_FILES}
    )

    install(
        TARGETS
            ${PROJECT_NAME}
            SDL2
            SDL2_image
            SDL2_mixer
            SDL2_ttf

        DESTINATION
            ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
    )
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL2::SDL2main
        ${PROJECT_NAME}_application
)

if(MSVC)
    include(msvc_config.cmake)
endif()

if(UNIX)
    include(install_launcher.cmake)
endif()

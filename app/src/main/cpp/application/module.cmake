set(MODULE_NAME application)
set(MODULE_DIR ${SOURCE_DIR}/${MODULE_NAME})
set(INCLUDE_DIR ${MODULE_DIR}/${MODULE_NAME})
set(MODULE_TARGET ${PROJECT_NAME}_${MODULE_NAME})

set(MODULE_SOURCES
    ${MODULE_DIR}/game.cpp
    ${MODULE_DIR}/window.cpp
    ${MODULE_DIR}/renderer.cpp
    ${MODULE_DIR}/audio.cpp
)

set(MODULE_HEADERS
    ${INCLUDE_DIR}/game.hpp
    ${INCLUDE_DIR}/window.hpp
    ${INCLUDE_DIR}/renderer.hpp
    ${INCLUDE_DIR}/audio.hpp
)

add_library(
    ${MODULE_TARGET} STATIC
        ${MODULE_SOURCES}
        ${MODULE_HEADERS}
)

target_include_directories(
    ${MODULE_TARGET} PUBLIC
        ${MODULE_DIR}
)

target_link_libraries(
    ${MODULE_TARGET} PUBLIC
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    ${PROJECT_NAME}_objects
)

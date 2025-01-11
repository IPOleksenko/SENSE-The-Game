set(MODULE_NAME utils)
set(MODULE_DIR ${SOURCE_DIR}/${MODULE_NAME})
set(INCLUDE_DIR ${MODULE_DIR}/${MODULE_NAME})
set(MODULE_TARGET ${PROJECT_NAME}_${MODULE_NAME})

set(MODULE_SOURCES
    ${MODULE_DIR}/texture.cpp
    ${MODULE_DIR}/waveform.cpp
)

set(MODULE_HEADERS
    ${INCLUDE_DIR}/texture.hpp
    ${INCLUDE_DIR}/waveform.hpp
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
        ${PROJECT_NAME}_assets
)

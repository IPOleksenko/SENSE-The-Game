set(MODULE_NAME objects)
set(MODULE_DIR ${SOURCE_DIR}/${MODULE_NAME})
set(INCLUDE_DIR ${MODULE_DIR}/${MODULE_NAME})
set(MODULE_TARGET ${PROJECT_NAME}_${MODULE_NAME})

set(MODULE_SOURCES
    ${MODULE_DIR}/text.cpp
    ${MODULE_DIR}/flora.cpp
    ${MODULE_DIR}/road.cpp
    ${MODULE_DIR}/background.cpp
    ${MODULE_DIR}/player.cpp
    ${MODULE_DIR}/scale.cpp
    ${MODULE_DIR}/reload.cpp
    ${MODULE_DIR}/endRender.cpp
)

set(MODULE_HEADERS
    ${INCLUDE_DIR}/text.hpp
    ${INCLUDE_DIR}/flora.hpp
    ${INCLUDE_DIR}/road.hpp
    ${INCLUDE_DIR}/background.hpp
    ${INCLUDE_DIR}/player.hpp
    ${INCLUDE_DIR}/scale.hpp
    ${INCLUDE_DIR}/reload.hpp
    ${INCLUDE_DIR}/endRender.hpp
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
        SDL2_ttf::SDL2_ttf
        ${PROJECT_NAME}_window
        ${PROJECT_NAME}_utils
)

set(MODULE_NAME utils)
set(MODULE_DIR ${SOURCE_DIR}/${MODULE_NAME})
set(INCLUDE_DIR ${MODULE_DIR}/${MODULE_NAME})
set(MODULE_TARGET ${PROJECT_NAME}_${MODULE_NAME})

set(MODULE_SOURCES
    ${MODULE_DIR}/camera.cpp
    ${MODULE_DIR}/soundtrack.cpp
    ${MODULE_DIR}/renderTilingTexture.cpp
)

set(MODULE_HEADERS
    ${INCLUDE_DIR}/camera.hpp
    ${INCLUDE_DIR}/soundtrack.hpp
    ${INCLUDE_DIR}/renderTilingTexture.hpp
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
        ${PROJECT_NAME}_window       
)

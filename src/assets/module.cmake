set(MODULE_NAME assets)
set(MODULE_DIR ${SOURCE_DIR}/${MODULE_NAME})
set(INCLUDE_DIR ${MODULE_DIR}/${MODULE_NAME})
set(MODULE_TARGET ${PROJECT_NAME}_${MODULE_NAME})

include(${MODULE_DIR}/autogen.cmake)

set(MODULE_SOURCES
    ${MODULE_DIR}/assets.cpp
)

set(MODULE_HEADERS
    ${INCLUDE_DIR}/assets.hpp
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

target_include_directories(
    ${MODULE_TARGET} PRIVATE
        ${ASSETS_DIR}
)

target_link_libraries(
    ${MODULE_TARGET} PUBLIC
        incbin::incbin_static
)

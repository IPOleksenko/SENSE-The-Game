set(MODULE_NAME objects)
set(MODULE_DIR ${SOURCE_DIR}/${MODULE_NAME})
set(INCLUDE_DIR ${MODULE_DIR}/${MODULE_NAME})
set(MODULE_TARGET ${PROJECT_NAME}_${MODULE_NAME})

set(MODULE_SOURCES
    ${MODULE_DIR}/player.cpp
    ${MODULE_DIR}/road.cpp
    ${MODULE_DIR}/scale.cpp
    ${MODULE_DIR}/flora.cpp
    ${MODULE_DIR}/background.cpp
    ${MODULE_DIR}/text.cpp
    ${MODULE_DIR}/end.cpp
)

set(MODULE_HEADERS
    ${INCLUDE_DIR}/player.hpp
    ${INCLUDE_DIR}/road.hpp
    ${INCLUDE_DIR}/scale.hpp
    ${INCLUDE_DIR}/flora.hpp
    ${INCLUDE_DIR}/background.hpp
    ${INCLUDE_DIR}/text.hpp
    ${INCLUDE_DIR}/end.hpp
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
        ${PROJECT_NAME}_assets
        ${PROJECT_NAME}_utils
)

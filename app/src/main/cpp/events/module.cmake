set(MODULE_NAME events)
set(MODULE_DIR ${SOURCE_DIR}/${MODULE_NAME})
set(INCLUDE_DIR ${MODULE_DIR}/${MODULE_NAME})
set(MODULE_TARGET ${PROJECT_NAME}_${MODULE_NAME})

set(MODULE_SOURCES
    ${MODULE_DIR}/keydownEvent.cpp
    ${MODULE_DIR}/playerKeydownEvent.cpp
)

set(MODULE_HEADERS
    ${INCLUDE_DIR}/keydownEvent.hpp
    ${INCLUDE_DIR}/playerKeydownEvent.hpp
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
        ${PROJECT_NAME}_window
        ${PROJECT_NAME}_objects
)

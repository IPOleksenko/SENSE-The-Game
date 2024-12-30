set(INCBIN_DATA ${MODULE_DIR}/incbin_data.cpp)

cmake_path(
    GET ASSET_SOURCE
    FILENAME ASSET_SOURCE_FILENAME
)
cmake_path(
    GET INCBIN_DATA
    FILENAME INCBIN_DATA_FILENAME
)
cmake_path(
    NATIVE_PATH MODULE_DIR
    NORMALIZE ASSET_SOURCE_LOCATION
)
cmake_path(
    NATIVE_PATH ASSETS_DIR
    NORMALIZE ASSET_FILES_LOCATION
)

add_custom_command(
    OUTPUT 
        ${INCBIN_DATA}
    COMMAND $<TARGET_FILE:incbin_tool> 
        "${ASSET_SOURCE_FILENAME}"
        "-I${ASSET_SOURCE_LOCATION}"
        "-I${ASSET_FILES_LOCATION}"
        "-o" "${INCBIN_DATA_FILENAME}"
    WORKING_DIRECTORY
        ${MODULE_DIR}
    DEPENDS
        incbin_tool
    COMMENT
        "Running incbin_tool..."
)
add_custom_target(
    ${MODULE_TARGET}_incbin
    DEPENDS ${INCBIN_DATA}
)

set(MODULE_SOURCES
    ${MODULE_SOURCES}
    ${INCBIN_DATA}
)

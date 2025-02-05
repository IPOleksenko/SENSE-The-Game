set(SOURCE_TEMPLATE ${MODULE_DIR}/templates/assets.cpp.template)
set(HEADER_TEMPLATE ${MODULE_DIR}/templates/assets.hpp.template)

set(ASSET_SOURCE ${MODULE_DIR}/assets.cpp)
set(ASSET_HEADER ${INCLUDE_DIR}/assets.hpp)

set(NEW_LINE    "\n")
set(SEMICOLON   "\;")

file(GLOB_RECURSE ASSETS ${ASSETS_DIR}/*)

foreach(ASSET_PATH IN LISTS ASSETS)
    cmake_path(
        RELATIVE_PATH ASSET_PATH 
        BASE_DIRECTORY ${ASSETS_DIR}
    )
    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" ASSET_TOKEN ${ASSET_PATH})
    string(TOUPPER ${ASSET_TOKEN} ASSET_TOKEN)

    set(ASSET_INCBIN_DECLARE
        "INCBIN(${ASSET_TOKEN}, \"${ASSET_PATH}\");\n" 
    )
    set(
        ASSET_DEFINE_TOKEN
        "#define ${ASSET_TOKEN} ${ASSET_TOKEN}\n"
    )
    set(ASSET_INCBIN_EXTERN
        "INCBIN_EXTERN(${ASSET_TOKEN});\n" 
    )

    string(APPEND INCBIN_DECLARE    "${ASSET_INCBIN_DECLARE}")
    string(APPEND DEFINE_TOKEN      "${ASSET_DEFINE_TOKEN}")
    string(APPEND INCBIN_EXTERN     "${ASSET_INCBIN_EXTERN}")
endforeach()

file(READ ${SOURCE_TEMPLATE} ASSET_SOURCE_BEGIN)
file(WRITE ${ASSET_SOURCE} "${ASSET_SOURCE_BEGIN}")

file(APPEND ${ASSET_SOURCE} "${NEW_LINE}")
file(APPEND ${ASSET_SOURCE} "${INCBIN_DECLARE}")


file(READ ${HEADER_TEMPLATE} ASSET_HEADER_BEGIN)
file(WRITE ${ASSET_HEADER} "${ASSET_HEADER_BEGIN}")

file(APPEND ${ASSET_HEADER} "${NEW_LINE}")
file(APPEND ${ASSET_HEADER} "${DEFINE_TOKEN}")

file(APPEND ${ASSET_HEADER} "${NEW_LINE}")
file(APPEND ${ASSET_HEADER} "${INCBIN_EXTERN}")

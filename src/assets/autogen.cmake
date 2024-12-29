
set(SOURCE_TEMPLATE ${MODULE_DIR}/templates/assets.cpp.template)
set(HEADER_TEMPLATE ${MODULE_DIR}/templates/assets.hpp.template)

set(ASSET_SOURCE ${MODULE_DIR}/assets.cpp)
set(ASSET_HEADER ${INCLUDE_DIR}/assets.hpp)

set(INCBIN_DECLARE_BEGIN    "INCBIN_MULTIPLE(\n")
set(INCBIN_DECLARE_END      ")\n")
set(INCBIN_EXTERN_BEGIN     "INCBIN_EXTERN_MULTIPLE(\n")
set(INCBIN_EXTERN_END       ")\n")
set(NEW_LINE                "\n")

string(APPEND INCBIN_DECLARE ${INCBIN_DECLARE_BEGIN})
string(APPEND INCBIN_EXTERN ${INCBIN_EXTERN_BEGIN})

file(GLOB_RECURSE ASSETS ${ASSETS_DIR}/*)

foreach(ASSET_PATH IN LISTS ASSETS)
    cmake_path(
        RELATIVE_PATH ASSET_PATH 
        BASE_DIRECTORY ${ASSETS_DIR}
    )
    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" ASSET_TOKEN ${ASSET_PATH})
    string(TOUPPER ${ASSET_TOKEN} ASSET_MACRO)

    set(ASSET_INCBIN_DECLARE
    "    (${ASSET_MACRO}, \"${ASSET_PATH}\")\n" 
    )
    set(
        ASSET_DEFINE_TOKEN
        "#define ${ASSET_MACRO} ${ASSET_TOKEN}\n"
    )
    set(ASSET_INCBIN_EXTERN
        "    (${ASSET_MACRO})\n" 
    )

    string(APPEND INCBIN_DECLARE    ${ASSET_INCBIN_DECLARE})
    string(APPEND DEFINE_TOKEN      ${ASSET_DEFINE_TOKEN})
    string(APPEND INCBIN_EXTERN     ${ASSET_INCBIN_EXTERN})
endforeach()

string(APPEND INCBIN_DECLARE    ${INCBIN_DECLARE_END})
string(APPEND INCBIN_EXTERN     ${INCBIN_EXTERN_END})


file(READ ${SOURCE_TEMPLATE} ASSET_SOURCE_BEGIN)
file(WRITE ${ASSET_SOURCE} ${ASSET_SOURCE_BEGIN})

file(APPEND ${ASSET_SOURCE} ${NEW_LINE})
file(APPEND ${ASSET_SOURCE} ${INCBIN_DECLARE})


file(READ ${HEADER_TEMPLATE} ASSET_HEADER_BEGIN)
file(WRITE ${ASSET_HEADER} ${ASSET_HEADER_BEGIN})

file(APPEND ${ASSET_HEADER} ${NEW_LINE})
file(APPEND ${ASSET_HEADER} ${DEFINE_TOKEN})

file(APPEND ${ASSET_HEADER} ${NEW_LINE})
file(APPEND ${ASSET_HEADER} ${INCBIN_EXTERN})
include(FetchContent)

FetchContent_Declare(
    incbin
    GIT_REPOSITORY https://github.com/Vortm4x/incbin.git
    GIT_TAG ce35df444687b7be51991fd4ec192d38b35acbbb
    SOURCE_DIR ${PROJECT_ROOT}/external/incbin
)

FetchContent_MakeAvailable(
    incbin
)

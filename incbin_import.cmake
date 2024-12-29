include(FetchContent)

FetchContent_Declare(
    incbin
    GIT_REPOSITORY https://github.com/Vortm4x/incbin.git
    GIT_TAG 9f4a3050e18da8ca5b6b2aa11e7f41c6fcd7114e
)

FetchContent_MakeAvailable(
    incbin
)

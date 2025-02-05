include(FetchContent)

cmake_policy(SET CMP0077 NEW)
set(SDL2MIXER_OPUS OFF)
set(SDL2MIXER_MOD OFF)
set(SDL2MIXER_MIDI OFF)
set(SDL2MIXER_WAVPACK OFF)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.10
    SOURCE_DIR ${PROJECT_ROOT}/external/SDL2
)
FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.8.3
    SOURCE_DIR ${PROJECT_ROOT}/external/SDL2_image
)
FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-2.22.0
    SOURCE_DIR ${PROJECT_ROOT}/external/SDL2_ttf
)
FetchContent_Declare(
    SDL2_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-2.8.0
    SOURCE_DIR ${PROJECT_ROOT}/external/SDL2_mixer
)


FetchContent_MakeAvailable(
    SDL2
    SDL2_image
    SDL2_ttf
    SDL2_mixer
)

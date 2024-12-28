cmake_minimum_required(VERSION 3.28.3)
project(SENSE_THE_GAME LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the path to the assets folder
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

# Include FetchContent to download dependencies
include(FetchContent)

# Fetch SDL2 and related modules
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.10 # Specify the required SDL2 version
)
FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.8.3 # Specify the required SDL2_image version
)
FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-2.22.0 # Specify the required SDL2_ttf version
)
FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_ttf)

# Collect source files
file(GLOB SOURCES "src/main.cpp")

# Create the main executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add dependencies and link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
)

# Specify include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${SDL2_SOURCE_DIR}/include
        ${SDL2_image_SOURCE_DIR}/include
        ${SDL2_ttf_SOURCE_DIR}/include
)

# Configure settings for MSVC
if(MSVC)
    # Set the runtime library to Multi-threaded DLL (/MD)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/MD> $<$<CONFIG:Debug>:/MDd>)

    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
        $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
    )
endif()

# Copy the assets folder to the build directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Copying the assets folder to the build directory"
)

# Copy DLL files to the build directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL2::SDL2> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL2_image::SDL2_image> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL2_ttf::SDL2_ttf> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMENT "Copying DLL files to the build directory"
)

# Set the default startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Configure the working directory for debugging in Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

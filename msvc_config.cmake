# Set the runtime library to Multi-threaded DLL (/MD)
target_compile_options(${PROJECT_NAME} 
    PRIVATE 
        $<$<CONFIG:Release>:/MD> 
        $<$<CONFIG:Debug>:/MDd>
)

# Enable console on debug
target_link_options(${PROJECT_NAME} 
    PRIVATE
        $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
        $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
)

# Set the default startup project in Visual Studio
set_directory_properties(
    ${CMAKE_CURRENT_SOURCE_DIR} PROPERTIES
    PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}
)

# Configure the working directory for debugging in Visual Studio
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

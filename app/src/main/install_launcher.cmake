set(SHELL_LAUNCHER_FILE ${PROJECT_ROOT}/${PROJECT_NAME}.sh)

file(
    WRITE
    ${SHELL_LAUNCHER_FILE}
    "export LD_LIBRARY_PATH=\$(pwd)\n./${PROJECT_NAME}\n"
)

install(
    FILES
        ${SHELL_LAUNCHER_FILE}
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
)

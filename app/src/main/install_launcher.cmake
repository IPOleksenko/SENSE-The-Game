set(SHELL_LAUNCHER_FILE ${PROJECT_ROOT}/${PROJECT_NAME}.sh)

if(ANDROID)
    file(
        WRITE
        ${SHELL_LAUNCHER_FILE}
        "export LD_LIBRARY_PATH=\$(pwd)\n./${PROJECT_NAME}\n"
    )

elseif(UNIX)
    file(
        WRITE
        ${SHELL_LAUNCHER_FILE}
        "#!/bin/bash\n"
        "BIN_DIR=\$( cd -- \"\$( dirname -- \"${BASH_SOURCE[0]}\" )\" &> /dev/null && pwd )\n"
        "echo \"Program directory: \$BIN_DIR\"\n"
        "export LD_LIBRARY_PATH=\$BIN_DIR\n"
        "\n"
        "# Install dpkg-dev if not already installed\n"
        "if ! command -v dpkg-architecture &> /dev/null; then\n"
        "    echo \"⏳ Installing dpkg-dev...\"\n"
        "    sudo apt update\n"
        "    sudo apt install -y dpkg-dev\n"
        "fi\n"
        "\n"
        "# Detect system architecture\n"
        "HOST_ARCH=\$(dpkg-architecture -q DEB_HOST_ARCH)\n"
        "GAME_BINARY=\"\$BIN_DIR/${PROJECT_NAME}\"\n"
        "chmod +x \$GAME_BINARY\n"
        "\n"
        "# Check if game binary exists and is executable\n"
        "if [[ ! -x \"\$GAME_BINARY\" ]]; then\n"
        "    echo \"❌ Error: \$GAME_BINARY does not exist or is not executable\"\n"
        "    exit 1\n"
        "fi\n"
        "\n"
        "# Detect game binary architecture\n"
        "BINARY_ARCH=\$(file \"\$GAME_BINARY\" | grep -oE '32-bit|64-bit')\n"
        "echo \"System architecture: \$HOST_ARCH\"\n"
        "echo \"Game binary architecture: \$BINARY_ARCH\"\n"
        "\n"
        "NEED_INSTALL=false\n"
        "install_common_dependencies() {\n"
        "    INSTALL_OUTPUT=\$(sudo apt install -y \\\n"
        "        libfreetype6\${1} \\\n"
        "        libasound2t64\${1} \\\n"
        "        libpulse0\${1} \\\n"
        "        libpulse-mainloop-glib0\${1} \\\n"
        "        libgles2\${1} \\\n"
        "        libgl1\${1} \\\n"
        "        libglvnd0\${1} \\\n"
        "        libglx0\${1} 2>&1)\n"
        "    echo \"\$INSTALL_OUTPUT\"\n"
        "    if echo \"\$INSTALL_OUTPUT\" | grep -qE 'Setting up|The following NEW packages will be installed|The following packages will be upgraded'; then\n"
        "        NEED_INSTALL=true\n"
        "    fi\n"
        "}\n"
        "\n"
        "if [[ \"\$BINARY_ARCH\" == \"32-bit\" && \"\$HOST_ARCH\" == \"amd64\" ]]; then\n"
        "    echo \"🔧 Enabling 32-bit library support...\"\n"
        "    sudo dpkg --add-architecture i386\n"
        "    sudo apt update\n"
        "    install_common_dependencies \":i386\"\n"
        "    INSTALL_LIBS_OUTPUT=\$(sudo apt install -y libc6:i386 libstdc++6:i386 2>&1)\n"
        "    echo \"\$INSTALL_LIBS_OUTPUT\"\n"
        "    if echo \"\$INSTALL_LIBS_OUTPUT\" | grep -qE 'Setting up|The following NEW packages will be installed|The following packages will be upgraded'; then\n"
        "        NEED_INSTALL=true\n"
        "    fi\n"
        "else\n"
        "    install_common_dependencies \"\"\n"
        "fi\n"
        "\n"
        "if \$NEED_INSTALL; then\n"
        "    echo \"📦 Installing dependencies...\"\n"
        "fi\n"
        "\n"
        "echo \"✅ Installation complete.\"\n"
        "echo \"🚀 Launching the game...\"\n"
        "SDL_RENDER_DRIVER=software \"\$GAME_BINARY\"\n"
    )
    
endif()

install(
    FILES
        ${SHELL_LAUNCHER_FILE}
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
)

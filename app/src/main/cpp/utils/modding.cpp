#include <utils/modding.hpp>
#include <SDL.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#ifdef __ANDROID__
#include <jni.h>
#endif

namespace modding {

// Constants
const char* LOCALIZATION_FILE = "localization.cfg";
const char* FONT_FILE = "font.cfg";
const char* DECOR_CFG = "decor.cfg";
const char* DECOR_DIR = "decor";

// Standard decor textures
const std::vector<std::string> STANDARD_DECOR = {
    "grass",
    "flower1",
    "flower2",
    "flower3",
    "flower4",
    "smallrock1",
    "smallrock2",
    "smallrock3"
};

// Standard localization keys from checkpoints.cpp
const std::vector<std::string> LOCALIZATION_KEYS = {
    "IDLE",
    "A_START",
    "B_START",
    "C_START",
    "D_START",
    "E_START",
    "F_START",
    "G_START",
    "H_START",
    "I_START",
    "J_START",
    "K_START",
    "L_START",
    "M_START",
    "N_START",
    "O_START",
    "P_START",
    "Q_START",
    "R_START",
    "S_START",
    "T_START",
    "FINAL_START",
    "LOADING_TEXT",
    "ENDLESS_MODE"
};

std::string getModdingDirectory() {
#ifdef __ANDROID__
    static std::string dir;

    if (dir.empty()) {
        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
        jobject activity = (jobject)SDL_AndroidGetActivity();
        if (!env || !activity) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get JNI env or activity");
            dir = ".";
            return dir;
        }

        jclass activityClass = env->GetObjectClass(activity);
        jmethodID midGetExternalMediaDirs = env->GetMethodID(
                activityClass, "getExternalMediaDirs", "()[Ljava/io/File;"
        );

        jobjectArray mediaDirs = (jobjectArray)env->CallObjectMethod(activity, midGetExternalMediaDirs);
        if (!mediaDirs) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "getExternalMediaDirs() returned null");
            dir = ".";
        } else {
            jobject firstDir = env->GetObjectArrayElement(mediaDirs, 0);
            if (firstDir) {
                jclass fileClass = env->FindClass("java/io/File");
                jmethodID midGetAbsolutePath = env->GetMethodID(fileClass, "getAbsolutePath", "()Ljava/lang/String;");
                jstring pathStr = (jstring)env->CallObjectMethod(firstDir, midGetAbsolutePath);

                const char* pathCStr = env->GetStringUTFChars(pathStr, nullptr);
                dir = pathCStr;
                env->ReleaseStringUTFChars(pathStr, pathCStr);

                env->DeleteLocalRef(pathStr);
                env->DeleteLocalRef(fileClass);
                env->DeleteLocalRef(firstDir);
            } else {
                dir = ".";
            }
            env->DeleteLocalRef(mediaDirs);
        }

        env->DeleteLocalRef(activityClass);
        env->DeleteLocalRef(activity);

        try {
            std::filesystem::create_directories(dir);
            SDL_Log("Modding directory: %s", dir.c_str());
        } catch (const std::exception& e) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create modding dir: %s", e.what());
        }
    }

    return dir;
#else
    return std::filesystem::current_path().string();
#endif
}

std::vector<std::string> readTextFile(const std::string& path) {
    std::vector<std::string> lines;
    try {
        std::ifstream file(path);
        if (!file.is_open()) {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Could not open file for reading: %s", path.c_str());
            return lines;
        }
        std::string line;
        while (std::getline(file, line)) {
            if (isCommentLine(line)) continue;
            lines.push_back(line);
        }
    } catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error reading file %s: %s", path.c_str(), e.what());
    }
    return lines;
}

bool writeTextFile(const std::string& path, const std::vector<std::string>& lines) {
    try {
        std::ofstream file(path, std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Could not open file for writing: %s", path.c_str());
            return false;
        }
        for (const auto& l : lines) {
            file << l << "\n";
        }
        return true;
    } catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error writing file %s: %s", path.c_str(), e.what());
        return false;
    }
}

bool fileExists(const std::string& path) {
    try {
        return std::filesystem::is_regular_file(path);
    } catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error checking file existence %s: %s", path.c_str(), e.what());
        return false;
    }
}

bool dirExists(const std::string& path) {
    try {
        return std::filesystem::is_directory(path);
    } catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error checking directory existence %s: %s", path.c_str(), e.what());
        return false;
    }
}

bool createDir(const std::string& path) {
    try {
        return std::filesystem::create_directories(path);
    } catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating directory %s: %s", path.c_str(), e.what());
        return false;
    }
}

std::string extractQuotedValue(const std::string& line) {
    size_t start = line.find('"');
    if (start == std::string::npos) return "";

    std::string result;
    for (size_t i = start + 1; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            size_t backslashes = 0;
            size_t j = i;
            while (j > start + 1 && line[j - 1] == '\\') {
                ++backslashes;
                --j;
            }
            if ((backslashes % 2) == 0) {
                break;
            }
        }
        result.push_back(c);
    }
    return result;
}

std::string extractValue(const std::string& line) {
    std::string value = line;
    value.erase(0, value.find_first_not_of(" \t"));
    value.erase(value.find_last_not_of(" \t") + 1);
    return value;
}

bool isCommentLine(const std::string& line) {
    return !line.empty() && line[0] == '#';
}

std::string joinPath(const std::string& base, const std::string& path) {
    return (std::filesystem::path(base) / path).string();
}

std::string unescapeString(const std::string& input) {
    std::string output;
    output.reserve(input.size());

    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '\\' && i + 1 < input.size()) {
            char next = input[i + 1];
            switch (next) {
            case 'n':
                output.push_back('\n');
                ++i;
                break;
            case 't':
                output.append("    ");
                ++i;
                break;
            case '\\':
                output.push_back('\\');
                ++i;
                break;
            case '"':
                output.push_back('"');
                ++i;
                break;
            default:
                output.push_back('\\');
                output.push_back(next);
                ++i;
                break;
            }
        }
        else {
            output.push_back(input[i]);
        }
    }
    return output;
}

// Localization
std::map<std::string, std::string> loadLocalization() {
    std::map<std::string, std::string> result;
    std::string path = joinPath(getModdingDirectory(), LOCALIZATION_FILE);

    if (!fileExists(path)) {
        createDefaultLocalizationFile();
        return result;
    }

    for (const auto& line : readTextFile(path)) {
        size_t pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string key = line.substr(0, pos);
        std::string value = extractQuotedValue(line.substr(pos + 1));

        if (!value.empty()) {
            result[key] = unescapeString(value);
        }
    }

    return result;
}

bool createDefaultLocalizationFile() {
    std::string path = joinPath(getModdingDirectory(), LOCALIZATION_FILE);
    if (fileExists(path)) {
        return true;  // File already exists
    }

    std::vector<std::string> lines;
    lines.push_back("# SENSE: The Game Localization File");
    lines.push_back("");
    lines.push_back("# Use KEY=\"value\" format to override default text.");
    lines.push_back("# Leave KEY= empty or \"\" to use default text.");
    lines.push_back("# If KEY= is not found, default settings are applied.");
    lines.push_back("# Supported escape sequences:");
    lines.push_back("#   \\n  - newline");
    lines.push_back("#   \\t  - tab (4 spaces)");
    lines.push_back("#   \\\"  - double quote");
    lines.push_back("#   \\\\  - backslash");
    lines.push_back("# Any unknown escape sequence after \\ will be written as-is.");
    lines.push_back("");


    for (const auto& key : LOCALIZATION_KEYS) {
        lines.push_back(key + "=");
    }

    if (writeTextFile(path, lines)) {
        SDL_Log("Created localization config file: %s", path.c_str());
        return true;
    }
    return false;
}

// Custom font
int fontSize = 24;
int otherTextFontSize = 48;

bool loadCustomFontSize() {
    std::string configPath = joinPath(getModdingDirectory(), FONT_FILE);
    auto lines = readTextFile(configPath);

    for (const auto& line : lines) {
        if (line.find("FONT_SIZE=") == 0) {
            std::string sizeStr = extractValue(line.substr(10));
            if (!sizeStr.empty() && std::all_of(sizeStr.begin(), sizeStr.end(), ::isdigit)) {
                try {
                    int size = std::stoi(sizeStr);
                    if (size > 0 && size <= 128) {
                        fontSize = size;
                        SDL_Log("Using custom FONT_SIZE: %d", fontSize);
                    }
                }
                catch (...) {
                    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                        "Failed to parse FONT_SIZE='%s' → using default %d",
                        sizeStr.c_str(), fontSize);
                }
            }
        }
        else if (line.find("OTHER_TEXT_FONT_SIZE=") == 0) {
            std::string sizeStr = extractValue(line.substr(21));
            if (!sizeStr.empty() && std::all_of(sizeStr.begin(), sizeStr.end(), ::isdigit)) {
                try {
                    int size = std::stoi(sizeStr);
                    if (size > 0 && size <= 128) {
                        otherTextFontSize = size;
                        SDL_Log("Using custom OTHER_TEXT_FONT_SIZE: %d", otherTextFontSize);
                    }
                }
                catch (...) {
                    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                        "Failed to parse OTHER_TEXT_FONT_SIZE='%s' → using default %d",
                        sizeStr.c_str(), otherTextFontSize);
                }
            }
        }
    }

    return true;
}

std::string loadCustomFontPath() {
    std::string configPath = joinPath(getModdingDirectory(), FONT_FILE);
    
    if (!fileExists(configPath)) {
        createDefaultFontFile();
        return "";
    }

    auto lines = readTextFile(configPath);
    for (const auto& line : lines) {
        if (line.find("FONT=") == 0) {
            std::string fontPath = extractQuotedValue(line.substr(5));
            if (!fontPath.empty()) {
                // If path is relative, prepend working directory
                if (fontPath[0] == '.' || !std::filesystem::path(fontPath).is_absolute()) {
                    fontPath = joinPath(getModdingDirectory(), fontPath);
                }

            }
            if (fileExists(fontPath)) {
                SDL_Log("Loading custom font: %s", fontPath.c_str());
                return fontPath;
            }
            else if(!fontPath.empty()) {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Custom font file not found: %s", fontPath.c_str());
            }
        }
    }

    return "";
}

bool createDefaultFontFile() {
    std::string path = joinPath(getModdingDirectory(), FONT_FILE);
    if (fileExists(path)) {
        return true;  // File already exists
    }

    std::vector<std::string> lines = {
        "# SENSE: The Game Font File",
        "",
        "# Use FONT=\"./path/to/font.ttf\" or FONT=\"font.otf\" to use a custom font.",
        "# Only fonts compatible with SDL2_ttf's TTF_OpenFont function are supported.",
        "# Leave FONT empty (FONT=\"\" or FONT=) to use the default font.",
        "# If the font is not found, the default font will be used.",
        "# If FONT= is not found, default settings are applied.",
        "",
        "FONT=\"\"",
        "",
        "",
        "# Leave FONT_SIZE= to use the default size.",
        "# If FONT_SIZE is not set, the default size is 24.",
        "# FONT_SIZE must contain only digits and be between 1 and 128 (e.g., FONT_SIZE=32); otherwise the default value will be used.",
        "# If FONT_SIZE= is not found, default settings are applied.",
        "",
        "FONT_SIZE=",
        "",
        "",
        "# Leave OTHER_TEXT_FONT_SIZE= to use the default size.",
        "# If OTHER_TEXT_FONT_SIZE is not set, the default size is 48.",
        "# OTHER_TEXT_FONT_SIZE must contain only digits and be between 1 and 128 (e.g., OTHER_TEXT_FONT_SIZE=32); otherwise the default value will be used.",
        "# If OTHER_TEXT_FONT_SIZE= is not found, default settings are applied.",
        "",
        "OTHER_TEXT_FONT_SIZE="
    };

    if (writeTextFile(path, lines)) {
        SDL_Log("Created font config file: %s", path.c_str());
        return true;
    }
    return false;
}

// Decor assets
std::vector<DecorAsset> loadDecorAssets() {
    std::vector<DecorAsset> assets;
    std::string decorPath = joinPath(getModdingDirectory(), DECOR_DIR);
    std::string configPath = joinPath(getModdingDirectory(), DECOR_CFG);

    // Read configuration
    std::map<std::string, bool> config;
    if (fileExists(configPath)) {
        auto lines = readTextFile(configPath);
        for (const auto& line : lines) {
            if (line.find('=') != std::string::npos) {
                std::string name = line.substr(0, line.find('='));
                std::string value = line.substr(line.find('=') + 1);
                config[name] = (value == "true");
            }
        }
    }

    // Load standard assets
    for (const auto& name : STANDARD_DECOR) {
        DecorAsset asset;
        asset.name = name;
        asset.enabled = config.count(name) ? config[name] : true;  // Use config value or true by default
        asset.isCustom = fileExists(joinPath(decorPath, name + ".png"));
        assets.push_back(asset);
    }

    // Search for custom assets
    try {
        for (const auto& entry : std::filesystem::directory_iterator(decorPath)) {
            if (entry.path().extension() != ".png") continue;
            
            std::string name = entry.path().stem().string();
            if (std::find_if(assets.begin(), assets.end(),
                [&name](const DecorAsset& a) { return a.name == name; }) == assets.end()) {
                // New asset
                DecorAsset asset;
                asset.name = name;
                asset.enabled = true;
                asset.isCustom = true;
                assets.push_back(asset);
            }
        }
    } catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error scanning decor directory: %s", e.what());
    }

    return assets;
}

bool createDecorDirectory() {
    const std::string dirPath = joinPath(getModdingDirectory(), DECOR_DIR);
    const std::string configPath = joinPath(getModdingDirectory(), DECOR_CFG);

    bool success = true;

    // Create directory if it doesn't exist
    if (!dirExists(dirPath)) {
        if (createDir(dirPath)) {
            SDL_Log("Created decor directory: %s", dirPath.c_str());
        } else {
            success = false;
        }
    }

    // Create config file if it doesn't exist
    if (!fileExists(configPath)) {
        std::vector<std::string> lines = {
            "# SENSE: The Game Decor Configuration",
            "",
            "# Use NAME=true/false to enable/disable decor assets.",
            "# If no value is set, the texture will not be displayed.",
            "# Add a custom .png image to the decor folder and give it a name different from the standard assets so it appears in the game.",
            "# Custom assets have the same functionality as standard ones.",
            "# You can also override a standard .png by giving it the same name.",
            "# If NAME= is not found, default settings are applied.",
            "",
            "# Standard decor assets:"
        };

        for (const auto& name : STANDARD_DECOR) {
            lines.push_back(name + "=true");
        }

        if (writeTextFile(configPath, lines)) {
            SDL_Log("Created decor config file: %s", configPath.c_str());
        } else {
            success = false;
        }
    }
    
    return success;
}

} // namespace modding

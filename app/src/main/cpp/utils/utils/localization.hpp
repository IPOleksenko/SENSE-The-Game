#pragma once
#include <string>
#include <map>

class LocalizationManager {
public:
    static LocalizationManager& instance() {
        static LocalizationManager inst;
        return inst;
    }

    void init();

    std::string getText(const std::string& key) const;

private:
    LocalizationManager() = default;
    std::map<std::string, std::string> m_texts;
};

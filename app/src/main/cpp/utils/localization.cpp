#include "utils/localization.hpp"
#include "utils/modding.hpp"
#include <fstream>

void LocalizationManager::init() {
    m_texts = modding::loadLocalization();
}

std::string LocalizationManager::getText(const std::string& key) const {
    auto it = m_texts.find(key);
    if (it != m_texts.end()) {
        return it->second;
    }
    return "";
}

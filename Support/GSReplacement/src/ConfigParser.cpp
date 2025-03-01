#include "../include/ConfigParser.h"
#include "../include/GameBackend.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace GSReplacement {

ConfigParser::ConfigParser() {
    // Initialize with default settings
    BackendConfigToSettings(BackendConfig::CreateDefaultRevora());
}

ConfigParser::~ConfigParser() {
    // Nothing to clean up
}

bool ConfigParser::LoadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    
    // Clear existing settings
    m_settings.clear();
    
    std::string line;
    while (std::getline(file, line)) {
        ParseLine(line);
    }
    
    file.close();
    return true;
}

bool ConfigParser::SaveToFile(const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    
    // Write header
    file << "# GameSpy Replacement Configuration" << std::endl;
    file << "# Generated automatically" << std::endl;
    file << std::endl;
    
    // Write settings
    for (const auto& pair : m_settings) {
        file << pair.first << " = " << pair.second << std::endl;
    }
    
    file.close();
    return true;
}

BackendConfig ConfigParser::GetBackendConfig() const {
    return SettingsToBackendConfig();
}

void ConfigParser::SetBackendConfig(const BackendConfig& config) {
    BackendConfigToSettings(config);
}

std::string ConfigParser::GetStringSetting(const std::string& key, const std::string& defaultValue) const {
    auto it = m_settings.find(key);
    if (it != m_settings.end()) {
        return it->second;
    }
    return defaultValue;
}

int ConfigParser::GetIntSetting(const std::string& key, int defaultValue) const {
    auto it = m_settings.find(key);
    if (it != m_settings.end()) {
        try {
            return std::stoi(it->second);
        } catch (const std::exception&) {
            return defaultValue;
        }
    }
    return defaultValue;
}

bool ConfigParser::GetBoolSetting(const std::string& key, bool defaultValue) const {
    auto it = m_settings.find(key);
    if (it != m_settings.end()) {
        std::string value = it->second;
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        if (value == "true" || value == "1" || value == "yes" || value == "y") {
            return true;
        } else if (value == "false" || value == "0" || value == "no" || value == "n") {
            return false;
        }
    }
    return defaultValue;
}

void ConfigParser::SetStringSetting(const std::string& key, const std::string& value) {
    m_settings[key] = value;
}

void ConfigParser::SetIntSetting(const std::string& key, int value) {
    m_settings[key] = std::to_string(value);
}

void ConfigParser::SetBoolSetting(const std::string& key, bool value) {
    m_settings[key] = value ? "true" : "false";
}

void ConfigParser::ParseLine(const std::string& line) {
    // Skip empty lines and comments
    if (line.empty() || line[0] == '#' || line[0] == ';') {
        return;
    }
    
    // Find the first equals sign
    size_t pos = line.find('=');
    if (pos == std::string::npos) {
        return;
    }
    
    // Extract key and value
    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + 1);
    
    // Trim whitespace
    key.erase(0, key.find_first_not_of(" \t"));
    key.erase(key.find_last_not_of(" \t") + 1);
    value.erase(0, value.find_first_not_of(" \t"));
    value.erase(value.find_last_not_of(" \t") + 1);
    
    // Store the setting
    if (!key.empty()) {
        m_settings[key] = value;
    }
}

BackendConfig ConfigParser::SettingsToBackendConfig() const {
    BackendConfig config;
    
    config.name = GetStringSetting("backend", "Revora");
    config.loginServerHost = GetStringSetting("loginServer", "gpcm.gamespy.com");
    config.loginServerPort = static_cast<uint16_t>(GetIntSetting("loginPort", 29900));
    config.chatServerHost = GetStringSetting("chatServer", "peerchat.gamespy.com");
    config.chatServerPort = static_cast<uint16_t>(GetIntSetting("chatPort", 6667));
    config.masterServerHost = GetStringSetting("masterServer", "ccgenzh.ms6.gamespy.com");
    config.masterServerPort = static_cast<uint16_t>(GetIntSetting("masterPort", 28900));
    config.natNegServerHost = GetStringSetting("natNegServer", "natneg1.gamespy.com");
    config.natNegServerPort = static_cast<uint16_t>(GetIntSetting("natNegPort", 27901));
    
    return config;
}

void ConfigParser::BackendConfigToSettings(const BackendConfig& config) {
    SetStringSetting("backend", config.name);
    SetStringSetting("loginServer", config.loginServerHost);
    SetIntSetting("loginPort", config.loginServerPort);
    SetStringSetting("chatServer", config.chatServerHost);
    SetIntSetting("chatPort", config.chatServerPort);
    SetStringSetting("masterServer", config.masterServerHost);
    SetIntSetting("masterPort", config.masterServerPort);
    SetStringSetting("natNegServer", config.natNegServerHost);
    SetIntSetting("natNegPort", config.natNegServerPort);
}

} // namespace GSReplacement 
#include "../include/GameBackend.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <mutex>
#include <sstream>

namespace GSReplacement {

// Static members
static std::ofstream s_logFile;
static Logger::LogLevel s_minLogLevel = Logger::LogLevel::INFO;
static std::mutex s_logMutex;
static bool s_initialized = false;

void Logger::Initialize(const std::string& logFilePath, LogLevel minLevel) {
    std::lock_guard<std::mutex> lock(s_logMutex);
    
    if (s_initialized) {
        // Already initialized, close the current log file
        if (s_logFile.is_open()) {
            s_logFile.close();
        }
    }
    
    s_logFile.open(logFilePath, std::ios::out | std::ios::app);
    if (!s_logFile.is_open()) {
        std::cerr << "Failed to open log file: " << logFilePath << std::endl;
        return;
    }
    
    s_minLogLevel = minLevel;
    s_initialized = true;
    
    // Log initialization
    Log(LogLevel::INFO, "Logger initialized. Log level: " + std::to_string(static_cast<int>(minLevel)));
}

void Logger::Shutdown() {
    std::lock_guard<std::mutex> lock(s_logMutex);
    
    if (s_initialized) {
        if (s_logFile.is_open()) {
            Log(LogLevel::INFO, "Logger shutting down.");
            s_logFile.close();
        }
        s_initialized = false;
    }
}

void Logger::Debug(const std::string& message) {
    Log(LogLevel::DEBUG, message);
}

void Logger::Info(const std::string& message) {
    Log(LogLevel::INFO, message);
}

void Logger::Warning(const std::string& message) {
    Log(LogLevel::WARNING, message);
}

void Logger::Error(const std::string& message) {
    Log(LogLevel::ERROR, message);
}

void Logger::Log(LogLevel level, const std::string& message) {
    if (!s_initialized || level < s_minLogLevel) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(s_logMutex);
    
    // Get current time
    time_t now = time(nullptr);
    struct tm timeInfo;
    char timeBuffer[80];
    
#ifdef _WIN32
    localtime_s(&timeInfo, &now);
#else
    localtime_r(&now, &timeInfo);
#endif
    
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
    
    // Convert log level to string
    std::string levelStr;
    switch (level) {
        case LogLevel::DEBUG:
            levelStr = "DEBUG";
            break;
        case LogLevel::INFO:
            levelStr = "INFO";
            break;
        case LogLevel::WARNING:
            levelStr = "WARNING";
            break;
        case LogLevel::ERROR:
            levelStr = "ERROR";
            break;
        default:
            levelStr = "UNKNOWN";
            break;
    }
    
    // Format the log message
    std::stringstream logMessage;
    logMessage << "[" << timeBuffer << "] [" << levelStr << "] " << message;
    
    // Write to log file
    if (s_logFile.is_open()) {
        s_logFile << logMessage.str() << std::endl;
        s_logFile.flush();
    }
    
    // Also print to console in debug mode
#ifdef _DEBUG
    std::cout << logMessage.str() << std::endl;
#endif
}

} // namespace GSReplacement 
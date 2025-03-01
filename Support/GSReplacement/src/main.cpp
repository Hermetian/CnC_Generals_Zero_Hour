#include "../include/GameBackend.h"
#include "../include/ConfigParser.h"
#include "../include/RevoraBackend.h"
#include "../include/FunctionHook.h"
#include "../include/GameSpyProxy.h"
#include <windows.h>
#include <string>
#include <iostream>

using namespace GSReplacement;

// Global variables
static GameBackend* g_backend = nullptr;
static ConfigParser g_configParser;
static std::string g_configPath = "GSReplacement.ini";

// Function to initialize the GameSpy replacement
bool InitializeGSReplacement() {
    // Initialize the logger
    Logger::Initialize("GSReplacement.log", Logger::LogLevel::DEBUG);
    Logger::Info("GameSpy Replacement Initializing");
    
    // Initialize function hooking
    if (!FunctionHook::Initialize()) {
        Logger::Error("Failed to initialize function hooking");
        return false;
    }
    
    // Load configuration
    if (!g_configParser.LoadFromFile(g_configPath)) {
        Logger::Warning("Failed to load config file. Using default Revora settings.");
        g_configParser.SetBackendConfig(BackendConfig::CreateDefaultRevora());
        g_configParser.SaveToFile(g_configPath);
    }
    
    // Create the backend
    BackendConfig config = g_configParser.GetBackendConfig();
    g_backend = GameBackend::CreateBackend(config.name);
    
    // Initialize the backend
    if (!g_backend->Initialize(config)) {
        Logger::Error("Failed to initialize backend");
        return false;
    }
    
    Logger::Info("GameSpy Replacement Initialized successfully");
    return true;
}

// Function to shutdown the GameSpy replacement
void ShutdownGSReplacement() {
    // Unhook all functions
    FunctionHook::Shutdown();
    
    // Shutdown backend
    if (g_backend) {
        g_backend->Shutdown();
        delete g_backend;
        g_backend = nullptr;
    }
    
    // Shutdown logger
    Logger::Shutdown();
}

#ifdef STANDALONE_APP
// Entry point when loaded as a standalone executable
int main(int argc, char** argv) {
    std::cout << "GameSpy Replacement for Command & Conquer: Generals - Zero Hour" << std::endl;
    std::cout << "This program can be used either as a standalone executable or as a DLL." << std::endl;
    
    // Initialize the GameSpy replacement
    if (!InitializeGSReplacement()) {
        std::cerr << "Failed to initialize GameSpy replacement." << std::endl;
        return 1;
    }
    
    std::cout << "GameSpy replacement initialized successfully." << std::endl;
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    
    // Shutdown the GameSpy replacement
    ShutdownGSReplacement();
    
    return 0;
}
#else
// Entry point when loaded as a DLL
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDLL);
            return GameSpyProxy::Initialize();
        case DLL_PROCESS_DETACH:
            GameSpyProxy::Shutdown();
            break;
    }
    return TRUE;
}
#endif 
#include "../include/GameSpyProxy.h"
#include "../include/GameBackend.h"
#include "../include/ConfigParser.h"
#include "../include/FunctionHook.h"
#include <windows.h>
#include <string>

namespace GSReplacement {

// Static member initialization
GameBackend* GameSpyProxy::s_backend = nullptr;
HINSTANCE GameSpyProxy::s_dllInstance = nullptr;

// Callback function pointers
static void* g_loginCallback = nullptr;
static void* g_gameListCallback = nullptr;
static void* g_hostGameCallback = nullptr;
static void* g_joinGameCallback = nullptr;
static void* g_inviteCallback = nullptr;

// Config
static std::string g_configPath;

bool GameSpyProxy::Initialize() {
    // Initialize the logger
    Logger::Initialize("GSReplacement.log", Logger::LogLevel::DEBUG);
    Logger::Info("GameSpy Proxy DLL Initializing");
    
    // Initialize function hooking
    if (!FunctionHook::Initialize()) {
        Logger::Error("Failed to initialize function hooking");
        return false;
    }
    
    // Load configuration
    g_configPath = "GSReplacement.ini";
    ConfigParser configParser;
    if (!configParser.LoadFromFile(g_configPath)) {
        Logger::Warning("Failed to load config file. Using default Revora settings.");
        configParser.SetBackendConfig(BackendConfig::CreateDefaultRevora());
        configParser.SaveToFile(g_configPath);
    }
    
    // Create the backend
    BackendConfig config = configParser.GetBackendConfig();
    s_backend = GameBackend::CreateBackend(config.name);
    
    // Initialize the backend
    if (!s_backend->Initialize(config)) {
        Logger::Error("Failed to initialize backend");
        return false;
    }
    
    // Register callbacks
    RegisterCallbacks();
    
    // Setup export functions
    SetupExportFunctions();
    
    Logger::Info("GameSpy Proxy DLL Initialized successfully");
    return true;
}

void GameSpyProxy::Shutdown() {
    // Unhook all functions
    FunctionHook::Shutdown();
    
    // Shutdown backend
    if (s_backend) {
        s_backend->Shutdown();
        delete s_backend;
        s_backend = nullptr;
    }
    
    // Shutdown logger
    Logger::Shutdown();
}

ResultCode GameSpyProxy::MapGameSpyErrorCode(int gameSpyErrorCode) {
    // Map GameSpy error code to our ResultCode
    switch (gameSpyErrorCode) {
        case 0: return ResultCode::SUCCESS;
        case 1: return ResultCode::ERROR_NETWORK;
        case 2: return ResultCode::ERROR_AUTH_FAILED;
        case 3: return ResultCode::ERROR_SERVER_UNAVAILABLE;
        case 4: return ResultCode::ERROR_TIMEOUT;
        case 5: return ResultCode::ERROR_NAT_FAILURE;
        // Add more mappings as needed
        default: return ResultCode::ERROR_UNKNOWN;
    }
}

int GameSpyProxy::MapToGameSpyErrorCode(ResultCode resultCode) {
    // Map our ResultCode to GameSpy error code
    switch (resultCode) {
        case ResultCode::SUCCESS: return 0;
        case ResultCode::ERROR_NETWORK: return 1;
        case ResultCode::ERROR_AUTH_FAILED: return 2;
        case ResultCode::ERROR_SERVER_UNAVAILABLE: return 3;
        case ResultCode::ERROR_TIMEOUT: return 4;
        case ResultCode::ERROR_NAT_FAILURE: return 5;
        // Add more mappings as needed
        default: return -1; // Unknown error
    }
}

void GameSpyProxy::SetupExportFunctions() {
    // Hook the original GameSpy functions
    // In a real implementation, we would find the function addresses
    // and hook them. For now, we'll just provide the export functions.
    
    // For example:
    // void* origLoginFunc = GetProcAddress(GetModuleHandle("gamespy.dll"), "GameSpy_Login");
    // FunctionHook::HookFunction(origLoginFunc, (void*)GameSpy_Login, "GameSpy_Login");
}

void GameSpyProxy::RegisterCallbacks() {
    // Register callbacks with the backend
    if (s_backend) {
        s_backend->RegisterInviteCallback([](ResultCode result, const std::string& fromPlayer, const GameSessionInfo& gameInfo) {
            if (g_inviteCallback) {
                // Call the original game's invite callback
                typedef void (*InviteCallbackType)(int result, const char* fromPlayer, int gameID);
                InviteCallbackType callback = (InviteCallbackType)g_inviteCallback;
                callback(MapToGameSpyErrorCode(result), fromPlayer.c_str(), gameInfo.sessionID);
            }
        });
    }
}

// Exported functions implementation

extern "C" {

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            GameSpyProxy::s_dllInstance = hinstDLL;
            DisableThreadLibraryCalls(hinstDLL);
            if (!GameSpyProxy::Initialize()) {
                return FALSE;
            }
            break;
        case DLL_PROCESS_DETACH:
            GameSpyProxy::Shutdown();
            break;
    }
    return TRUE;
}

__declspec(dllexport) int __stdcall GameSpy_Init(void* params) {
    Logger::Info("GameSpy_Init called");
    return 0; // Success
}

__declspec(dllexport) int __stdcall GameSpy_Login(const char* username, const char* password, void* callback) {
    Logger::Info("GameSpy_Login called for user: " + std::string(username));
    
    // Save the callback
    g_loginCallback = callback;
    
    if (GameSpyProxy::GetBackend()) {
        GameSpyProxy::GetBackend()->Login(username, password, [](ResultCode result, const PlayerInfo& playerInfo) {
            if (g_loginCallback) {
                typedef void (*LoginCallbackType)(int result, const char* username, int profileID);
                LoginCallbackType callback = (LoginCallbackType)g_loginCallback;
                callback(GameSpyProxy::MapToGameSpyErrorCode(result),
                         playerInfo.username.c_str(),
                         playerInfo.profileID);
            }
        });
    }
    
    return 0; // Success
}

__declspec(dllexport) int __stdcall GameSpy_GetGameList(void* callback) {
    Logger::Info("GameSpy_GetGameList called");
    
    // Save the callback
    g_gameListCallback = callback;
    
    if (GameSpyProxy::GetBackend()) {
        GameSpyProxy::GetBackend()->FetchGameList([](ResultCode result, const std::vector<GameSessionInfo>& gameList) {
            if (g_gameListCallback) {
                typedef void (*GameListCallbackType)(int result, int gameCount, void* gameList);
                GameListCallbackType callback = (GameListCallbackType)g_gameListCallback;
                
                // In a real implementation, we would convert the game list to a format
                // that the original game expects. For now, we'll just pass a dummy value.
                callback(GameSpyProxy::MapToGameSpyErrorCode(result),
                         gameList.size(),
                         (void*)&gameList);
            }
        });
    }
    
    return 0; // Success
}

__declspec(dllexport) int __stdcall GameSpy_HostGame(const char* gameName, int maxPlayers, void* callback) {
    Logger::Info("GameSpy_HostGame called for game: " + std::string(gameName));
    
    // Save the callback
    g_hostGameCallback = callback;
    
    if (GameSpyProxy::GetBackend()) {
        GameSessionInfo gameInfo;
        gameInfo.sessionName = gameName;
        gameInfo.maxPlayers = maxPlayers;
        gameInfo.curPlayers = 1; // Host is the first player
        gameInfo.hostPort = 16000; // Default port for Generals
        
        GameSpyProxy::GetBackend()->HostGame(gameInfo, [](ResultCode result, const GameSessionInfo& gameInfo) {
            if (g_hostGameCallback) {
                typedef void (*HostGameCallbackType)(int result, int gameID);
                HostGameCallbackType callback = (HostGameCallbackType)g_hostGameCallback;
                callback(GameSpyProxy::MapToGameSpyErrorCode(result),
                         gameInfo.sessionID);
            }
        });
    }
    
    return 0; // Success
}

__declspec(dllexport) int __stdcall GameSpy_JoinGame(int gameID, void* callback) {
    Logger::Info("GameSpy_JoinGame called for game ID: " + std::to_string(gameID));
    
    // Save the callback
    g_joinGameCallback = callback;
    
    if (GameSpyProxy::GetBackend()) {
        // Find the game in the list
        GameSessionInfo gameInfo;
        gameInfo.sessionID = gameID;
        
        GameSpyProxy::GetBackend()->JoinGame(gameInfo, [](ResultCode result, const GameSessionInfo& gameInfo) {
            if (g_joinGameCallback) {
                typedef void (*JoinGameCallbackType)(int result, int gameID, const char* hostIP, int hostPort);
                JoinGameCallbackType callback = (JoinGameCallbackType)g_joinGameCallback;
                callback(GameSpyProxy::MapToGameSpyErrorCode(result),
                         gameInfo.sessionID,
                         gameInfo.hostIP.c_str(),
                         gameInfo.hostPort);
            }
        });
    }
    
    return 0; // Success
}

__declspec(dllexport) int __stdcall GameSpy_SendInvite(const char* username, int gameID) {
    Logger::Info("GameSpy_SendInvite called for user: " + std::string(username) + " to game ID: " + std::to_string(gameID));
    
    if (GameSpyProxy::GetBackend()) {
        GameSpyProxy::GetBackend()->SendInvite(username, gameID);
    }
    
    return 0; // Success
}

__declspec(dllexport) int __stdcall GameSpy_Think() {
    // This function is called periodically by the game to process network events
    if (GameSpyProxy::GetBackend()) {
        GameSpyProxy::GetBackend()->PumpEvents();
    }
    
    return 0; // Success
}

} // extern "C"

} // namespace GSReplacement 
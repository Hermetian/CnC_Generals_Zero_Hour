#include "../include/RevoraBackend.h"
#include <ws2tcpip.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <regex>
#include <random>
#include <chrono>
#include <algorithm>

// Link with Winsock2 library
#pragma comment(lib, "ws2_32.lib")

namespace GSReplacement {

// Helper function to generate MD5 hash (simplified for brevity)
std::string GenerateMD5Hash(const std::string& input) {
    // In a real implementation, you'd use a proper MD5 library
    // For now, this is just a placeholder
    
    // This is a very simplified version that doesn't actually compute MD5
    // In production code, use a real MD5 implementation
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    
    // Generate a hash-like string (not actual MD5)
    for (size_t i = 0; i < 16; ++i) {
        ss << std::setw(2) << (int)(input[i % input.size()] + i);
    }
    
    return ss.str();
}

// Helper function to generate a random string
std::string GenerateRandomString(size_t length) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, sizeof(alphanum) - 2);
    
    std::string result;
    result.reserve(length);
    
    for (size_t i = 0; i < length; ++i) {
        result += alphanum[distribution(generator)];
    }
    
    return result;
}

RevoraBackend::RevoraBackend()
    : m_loginSocket(INVALID_SOCKET)
    , m_masterSocket(INVALID_SOCKET)
    , m_chatSocket(INVALID_SOCKET)
    , m_natSocket(INVALID_SOCKET)
    , m_loginCallback(nullptr)
    , m_gameListCallback(nullptr)
    , m_joinGameCallback(nullptr)
    , m_hostGameCallback(nullptr)
    , m_inviteCallback(nullptr)
    , m_threadRunning(false) {
    
    // Initialize members
    m_natInfo.targetPort = 0;
    m_natInfo.localPort = 0;
}

RevoraBackend::~RevoraBackend() {
    Shutdown();
}

bool RevoraBackend::Initialize(const BackendConfig& config) {
    // Save the configuration
    m_config = config;
    
    // Initialize WinSock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        Logger::Error("WSAStartup failed: " + std::to_string(result));
        return false;
    }
    
    Logger::Info("RevoraBackend initialized with backend: " + config.name);
    
    // Start background thread
    m_threadRunning = true;
    m_backgroundThread = std::thread(&RevoraBackend::BackgroundThreadFunc, this);
    
    return true;
}

void RevoraBackend::Shutdown() {
    // Stop background thread
    if (m_threadRunning) {
        m_threadRunning = false;
        if (m_backgroundThread.joinable()) {
            m_backgroundThread.join();
        }
    }
    
    // Close all sockets
    CloseSocket(m_loginSocket);
    CloseSocket(m_masterSocket);
    CloseSocket(m_chatSocket);
    CloseSocket(m_natSocket);
    
    // Cleanup WinSock
    WSACleanup();
    
    Logger::Info("RevoraBackend shutdown complete");
}

void RevoraBackend::Login(const std::string& username, const std::string& password, LoginCallback callback) {
    // Save the callback
    m_loginCallback = callback;
    
    // Reset local player info
    m_localPlayer = PlayerInfo();
    m_localPlayer.username = username;
    
    // Connect to login server
    if (!ConnectToServer(m_loginSocket, m_config.loginServerHost, m_config.loginServerPort)) {
        if (m_loginCallback) {
            m_loginCallback(ResultCode::ERROR_SERVER_UNAVAILABLE, m_localPlayer);
        }
        return;
    }
    
    // Send login request
    SendLoginRequest(username, password);
    
    Logger::Info("Login request sent for user: " + username);
}

void RevoraBackend::Logout() {
    // Close login socket
    CloseSocket(m_loginSocket);
    
    // Reset local player info
    m_localPlayer.online = false;
    
    Logger::Info("User logged out: " + m_localPlayer.username);
}

void RevoraBackend::FetchGameList(GameListCallback callback) {
    // Save the callback
    m_gameListCallback = callback;
    
    // Connect to master server if not already connected
    if (m_masterSocket == INVALID_SOCKET) {
        if (!ConnectToServer(m_masterSocket, m_config.masterServerHost, m_config.masterServerPort)) {
            if (m_gameListCallback) {
                m_gameListCallback(ResultCode::ERROR_SERVER_UNAVAILABLE, m_gameList);
            }
            return;
        }
    }
    
    // Send game list request
    SendGameListRequest();
    
    Logger::Info("Game list request sent");
}

void RevoraBackend::HostGame(const GameSessionInfo& gameInfo, HostGameCallback callback) {
    // Save the callback
    m_hostGameCallback = callback;
    
    // Save the game info
    m_hostedGame = gameInfo;
    
    // Connect to master server if not already connected
    if (m_masterSocket == INVALID_SOCKET) {
        if (!ConnectToServer(m_masterSocket, m_config.masterServerHost, m_config.masterServerPort)) {
            if (m_hostGameCallback) {
                m_hostGameCallback(ResultCode::ERROR_SERVER_UNAVAILABLE, m_hostedGame);
            }
            return;
        }
    }
    
    // Send host game request
    SendHostGameRequest(gameInfo);
    
    Logger::Info("Host game request sent for game: " + gameInfo.sessionName);
}

void RevoraBackend::CancelHost() {
    // Send cancel host request if we're hosting
    if (m_masterSocket != INVALID_SOCKET && m_hostedGame.sessionID != 0) {
        // TODO: Implement cancel host request
        
        // Clear hosted game info
        m_hostedGame = GameSessionInfo();
        
        Logger::Info("Host game canceled");
    }
}

void RevoraBackend::JoinGame(const GameSessionInfo& gameInfo, JoinGameCallback callback) {
    // Save the callback
    m_joinGameCallback = callback;
    
    // Start NAT negotiation if needed
    StartNatNegotiation(gameInfo);
    
    Logger::Info("Join game request sent for game: " + gameInfo.sessionName);
}

void RevoraBackend::SendInvite(const std::string& targetUser, uint32_t sessionID) {
    // TODO: Implement send invite
    
    Logger::Info("Invite sent to user: " + targetUser + " for game ID: " + std::to_string(sessionID));
}

void RevoraBackend::RegisterInviteCallback(InviteCallback callback) {
    m_inviteCallback = callback;
}

void RevoraBackend::PumpEvents() {
    // This is mostly handled by the background thread
    // But we can use this to check for any pending callbacks or messages
    
    // Process any pending messages
    // ...
}

const PlayerInfo& RevoraBackend::GetLocalPlayer() const {
    return m_localPlayer;
}

bool RevoraBackend::ConnectToServer(SOCKET& socket, const std::string& hostname, uint16_t port) {
    // Close existing socket if any
    CloseSocket(socket);
    
    // Create a new socket
    socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket == INVALID_SOCKET) {
        Logger::Error("Socket creation failed: " + std::to_string(WSAGetLastError()));
        return false;
    }
    
    // Resolve hostname
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    struct addrinfo* result = nullptr;
    int error = getaddrinfo(hostname.c_str(), std::to_string(port).c_str(), &hints, &result);
    if (error != 0) {
        Logger::Error("getaddrinfo failed: " + std::to_string(error));
        CloseSocket(socket);
        return false;
    }
    
    // Try to connect
    error = connect(socket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);
    
    if (error == SOCKET_ERROR) {
        Logger::Error("connect failed: " + std::to_string(WSAGetLastError()));
        CloseSocket(socket);
        return false;
    }
    
    // Set to non-blocking mode
    u_long mode = 1;
    ioctlsocket(socket, FIONBIO, &mode);
    
    Logger::Info("Connected to " + hostname + ":" + std::to_string(port));
    return true;
}

void RevoraBackend::CloseSocket(SOCKET& socket) {
    if (socket != INVALID_SOCKET) {
        closesocket(socket);
        socket = INVALID_SOCKET;
    }
}

void RevoraBackend::SendLoginRequest(const std::string& username, const std::string& password) {
    // GameSpy login protocol:
    // 1. Client connects to server
    // 2. Server sends a challenge string
    // 3. Client computes MD5 hash of (challenge + password)
    // 4. Client sends login request with username and hash
    
    // For now, we'll just send a simplified login request
    std::string loginRequest = "\\login\\\\user\\" + username + "\\password\\" + password + "\\gamename\\ccgenzh\\id\\1\\final\\";
    send(m_loginSocket, loginRequest.c_str(), (int)loginRequest.size(), 0);
    
    // In a real implementation, we would:
    // 1. Wait for the challenge from the server
    // 2. Compute the hash of (challenge + password)
    // 3. Send the login request with the hash
}

void RevoraBackend::ProcessLoginResponse(const std::string& response) {
    // Check if login was successful
    if (response.find("\\error\\") != std::string::npos) {
        // Extract error message
        std::regex errorRegex("\\\\error\\\\(.+?)\\\\");
        std::smatch match;
        std::string errorMsg;
        
        if (std::regex_search(response, match, errorRegex) && match.size() > 1) {
            errorMsg = match[1].str();
        } else {
            errorMsg = "Unknown error";
        }
        
        Logger::Error("Login failed: " + errorMsg);
        
        if (m_loginCallback) {
            m_loginCallback(ResultCode::ERROR_AUTH_FAILED, m_localPlayer);
        }
        return;
    }
    
    // Extract profile ID
    std::regex profileRegex("\\\\profileid\\\\(\\d+)");
    std::smatch match;
    
    if (std::regex_search(response, match, profileRegex) && match.size() > 1) {
        m_localPlayer.profileID = std::stoul(match[1].str());
    }
    
    // Mark as logged in
    m_localPlayer.online = true;
    
    Logger::Info("Login successful. Profile ID: " + std::to_string(m_localPlayer.profileID));
    
    if (m_loginCallback) {
        m_loginCallback(ResultCode::SUCCESS, m_localPlayer);
    }
}

std::string RevoraBackend::HashPassword(const std::string& password, const std::string& challenge) {
    // In a real implementation, this would compute MD5(challenge + password)
    return GenerateMD5Hash(challenge + password);
}

void RevoraBackend::SendGameListRequest() {
    // GameSpy master server protocol:
    // 1. Send a query packet to the master server
    // 2. Server responds with a list of game server IPs
    // 3. Client queries each game server for details
    
    // For now, send a simplified request
    std::string request = "\\list\\gamename\\ccgenzh\\final\\";
    send(m_masterSocket, request.c_str(), (int)request.size(), 0);
}

void RevoraBackend::ProcessGameListResponse(const std::string& response) {
    // Clear the game list
    m_gameList.clear();
    
    // In a real implementation, we would:
    // 1. Parse the list of game server IPs
    // 2. Query each server for details
    // 3. Add each game to the list
    
    // For now, create a dummy game list for testing
    for (int i = 0; i < 3; ++i) {
        GameSessionInfo gameInfo;
        gameInfo.sessionID = 1000 + i;
        gameInfo.hostName = "Host" + std::to_string(i);
        gameInfo.hostIP = "192.168.1." + std::to_string(10 + i);
        gameInfo.hostPort = 16000;
        gameInfo.sessionName = "Test Game " + std::to_string(i);
        gameInfo.curPlayers = 1;
        gameInfo.maxPlayers = 8;
        gameInfo.requiresPassword = (i % 2 == 0);
        
        m_gameList.push_back(gameInfo);
    }
    
    Logger::Info("Received game list with " + std::to_string(m_gameList.size()) + " games");
    
    if (m_gameListCallback) {
        m_gameListCallback(ResultCode::SUCCESS, m_gameList);
    }
}

void RevoraBackend::SendHostGameRequest(const GameSessionInfo& gameInfo) {
    // GameSpy hosting protocol:
    // 1. Register the game with the master server
    // 2. Send heartbeats to keep it alive
    
    // Generate a unique session ID if not provided
    if (m_hostedGame.sessionID == 0) {
        // Generate a "random" session ID
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<uint32_t> distribution(1000, 9999999);
        m_hostedGame.sessionID = distribution(generator);
    }
    
    // For now, send a simplified request
    std::stringstream request;
    request << "\\heartbeat\\"
            << "\\gamename\\ccgenzh"
            << "\\gamever\\1.04"
            << "\\hostname\\" << m_localPlayer.username
            << "\\hostport\\" << m_hostedGame.hostPort
            << "\\mapname\\Alpine Assault"
            << "\\numplayers\\" << (int)m_hostedGame.curPlayers
            << "\\maxplayers\\" << (int)m_hostedGame.maxPlayers
            << "\\gamemode\\openplaying"
            << "\\password\\" << (m_hostedGame.requiresPassword ? "1" : "0")
            << "\\final\\";
    
    std::string requestStr = request.str();
    send(m_masterSocket, requestStr.c_str(), (int)requestStr.size(), 0);
    
    // Start sending heartbeats
    // In a real implementation, we would start a timer to send periodic heartbeats
}

void RevoraBackend::ProcessHostGameResponse(const std::string& response) {
    // Check if hosting was successful
    if (response.find("\\error\\") != std::string::npos) {
        // Extract error message
        std::regex errorRegex("\\\\error\\\\(.+?)\\\\");
        std::smatch match;
        std::string errorMsg;
        
        if (std::regex_search(response, match, errorRegex) && match.size() > 1) {
            errorMsg = match[1].str();
        } else {
            errorMsg = "Unknown error";
        }
        
        Logger::Error("Host game failed: " + errorMsg);
        
        if (m_hostGameCallback) {
            m_hostGameCallback(ResultCode::ERROR_UNKNOWN, m_hostedGame);
        }
        return;
    }
    
    Logger::Info("Host game successful. Session ID: " + std::to_string(m_hostedGame.sessionID));
    
    if (m_hostGameCallback) {
        m_hostGameCallback(ResultCode::SUCCESS, m_hostedGame);
    }
}

void RevoraBackend::SendHeartbeat() {
    // Only send heartbeat if we're hosting
    if (m_masterSocket != INVALID_SOCKET && m_hostedGame.sessionID != 0) {
        // Send heartbeat to keep the game listing alive
        // Similar to the host game request, but with \heartbeat\ command
        
        std::stringstream request;
        request << "\\heartbeat\\"
                << "\\gamename\\ccgenzh"
                << "\\sesskey\\" << m_hostedGame.sessionID
                << "\\final\\";
        
        std::string requestStr = request.str();
        send(m_masterSocket, requestStr.c_str(), (int)requestStr.size(), 0);
        
        Logger::Debug("Heartbeat sent for game ID: " + std::to_string(m_hostedGame.sessionID));
    }
}

void RevoraBackend::StartNatNegotiation(const GameSessionInfo& gameInfo) {
    // Connect to NAT negotiation server
    if (!ConnectToServer(m_natSocket, m_config.natNegServerHost, m_config.natNegServerPort)) {
        if (m_joinGameCallback) {
            m_joinGameCallback(ResultCode::ERROR_SERVER_UNAVAILABLE, gameInfo);
        }
        return;
    }
    
    // Setup NAT negotiation info
    m_natInfo.cookie = GenerateRandomString(10);
    m_natInfo.targetIP = gameInfo.hostIP;
    m_natInfo.targetPort = gameInfo.hostPort;
    m_natInfo.localPort = 16500; // Local port for the game connection
    
    // Send NAT negotiation request
    std::stringstream request;
    request << "\\natneg\\"
            << "\\cookie\\" << m_natInfo.cookie
            << "\\ip\\" << m_natInfo.targetIP
            << "\\port\\" << m_natInfo.targetPort
            << "\\localport\\" << m_natInfo.localPort
            << "\\final\\";
    
    std::string requestStr = request.str();
    send(m_natSocket, requestStr.c_str(), (int)requestStr.size(), 0);
    
    Logger::Info("NAT negotiation started for game: " + gameInfo.sessionName);
}

void RevoraBackend::ProcessNatNegotiationResponse(const std::string& response) {
    // Check if NAT negotiation was successful
    if (response.find("\\error\\") != std::string::npos) {
        // Extract error message
        std::regex errorRegex("\\\\error\\\\(.+?)\\\\");
        std::smatch match;
        std::string errorMsg;
        
        if (std::regex_search(response, match, errorRegex) && match.size() > 1) {
            errorMsg = match[1].str();
        } else {
            errorMsg = "Unknown error";
        }
        
        Logger::Error("NAT negotiation failed: " + errorMsg);
        
        if (m_joinGameCallback) {
            GameSessionInfo gameInfo;
            gameInfo.hostIP = m_natInfo.targetIP;
            gameInfo.hostPort = m_natInfo.targetPort;
            m_joinGameCallback(ResultCode::ERROR_NAT_FAILURE, gameInfo);
        }
        return;
    }
    
    // Extract the negotiated connection details
    std::regex ipRegex("\\\\ip\\\\([\\d\\.]+)");
    std::regex portRegex("\\\\port\\\\(\\d+)");
    std::smatch match;
    
    std::string negotiatedIP = m_natInfo.targetIP;
    uint16_t negotiatedPort = m_natInfo.targetPort;
    
    if (std::regex_search(response, match, ipRegex) && match.size() > 1) {
        negotiatedIP = match[1].str();
    }
    
    if (std::regex_search(response, match, portRegex) && match.size() > 1) {
        negotiatedPort = static_cast<uint16_t>(std::stoi(match[1].str()));
    }
    
    Logger::Info("NAT negotiation successful. Connecting to: " + negotiatedIP + ":" + std::to_string(negotiatedPort));
    
    // Create the game info for the callback
    GameSessionInfo gameInfo;
    gameInfo.hostIP = negotiatedIP;
    gameInfo.hostPort = negotiatedPort;
    
    if (m_joinGameCallback) {
        m_joinGameCallback(ResultCode::SUCCESS, gameInfo);
    }
}

void RevoraBackend::SendInviteRequest(const std::string& targetUser, uint32_t sessionID) {
    // TODO: Implement sending invite request
    // This would typically be done through the chat server or a separate messaging system
}

void RevoraBackend::ProcessInviteResponse(const std::string& response) {
    // TODO: Implement processing invite response
}

void RevoraBackend::BackgroundThreadFunc() {
    Logger::Info("Background thread started");
    
    // Buffer for receiving data
    char buffer[4096];
    
    // Last heartbeat time
    auto lastHeartbeatTime = std::chrono::steady_clock::now();
    
    while (m_threadRunning) {
        // Set up fd_set for select
        fd_set readSet;
        FD_ZERO(&readSet);
        
        // Add all valid sockets to the set
        if (m_loginSocket != INVALID_SOCKET) {
            FD_SET(m_loginSocket, &readSet);
        }
        if (m_masterSocket != INVALID_SOCKET) {
            FD_SET(m_masterSocket, &readSet);
        }
        if (m_chatSocket != INVALID_SOCKET) {
            FD_SET(m_chatSocket, &readSet);
        }
        if (m_natSocket != INVALID_SOCKET) {
            FD_SET(m_natSocket, &readSet);
        }
        
        // Set up timeout (100ms)
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;
        
        // Wait for activity on any socket
        int result = select(0, &readSet, nullptr, nullptr, &timeout);
        
        if (result == SOCKET_ERROR) {
            Logger::Error("select failed: " + std::to_string(WSAGetLastError()));
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        
        // Check each socket for activity
        if (m_loginSocket != INVALID_SOCKET && FD_ISSET(m_loginSocket, &readSet)) {
            int bytesRead = recv(m_loginSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                ProcessLoginResponse(buffer);
            }
            else if (bytesRead == 0 || WSAGetLastError() != WSAEWOULDBLOCK) {
                // Connection closed or error
                Logger::Warning("Login connection closed");
                CloseSocket(m_loginSocket);
            }
        }
        
        if (m_masterSocket != INVALID_SOCKET && FD_ISSET(m_masterSocket, &readSet)) {
            int bytesRead = recv(m_masterSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                ProcessGameListResponse(buffer);
            }
            else if (bytesRead == 0 || WSAGetLastError() != WSAEWOULDBLOCK) {
                // Connection closed or error
                Logger::Warning("Master server connection closed");
                CloseSocket(m_masterSocket);
            }
        }
        
        if (m_chatSocket != INVALID_SOCKET && FD_ISSET(m_chatSocket, &readSet)) {
            int bytesRead = recv(m_chatSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                // Process chat messages
                // TODO: Implement chat message processing
            }
            else if (bytesRead == 0 || WSAGetLastError() != WSAEWOULDBLOCK) {
                // Connection closed or error
                Logger::Warning("Chat server connection closed");
                CloseSocket(m_chatSocket);
            }
        }
        
        if (m_natSocket != INVALID_SOCKET && FD_ISSET(m_natSocket, &readSet)) {
            int bytesRead = recv(m_natSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                ProcessNatNegotiationResponse(buffer);
            }
            else if (bytesRead == 0 || WSAGetLastError() != WSAEWOULDBLOCK) {
                // Connection closed or error
                Logger::Warning("NAT negotiation connection closed");
                CloseSocket(m_natSocket);
            }
        }
        
        // Check if it's time to send a heartbeat
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastHeartbeatTime).count();
        
        if (elapsedSeconds >= 20) { // Send heartbeat every 20 seconds
            SendHeartbeat();
            lastHeartbeatTime = currentTime;
        }
        
        // Sleep a bit to avoid hogging CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    Logger::Info("Background thread stopped");
}

} // namespace GSReplacement 
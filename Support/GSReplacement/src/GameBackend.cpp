#include "../include/GameBackend.h"
#include "../include/RevoraBackend.h"
#include <algorithm>
#include <cctype>

namespace GSReplacement {

// Factory method to create backend based on name
GameBackend* GameBackend::CreateBackend(const std::string& backendName) {
    // Convert to lowercase for case-insensitive comparison
    std::string name = backendName;
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c){ return std::tolower(c); });
    
    // Create the appropriate backend
    if (name == "revora") {
        Logger::Info("Creating Revora backend");
        return new RevoraBackend();
    }
    else if (name == "openspy") {
        Logger::Warning("OpenSpy backend not fully implemented yet, falling back to Revora");
        return new RevoraBackend(); // For now, use Revora for OpenSpy too
    }
    else {
        Logger::Warning("Unknown backend: " + backendName + ", using Revora");
        return new RevoraBackend(); // Default to Revora
    }
}

} // namespace GSReplacement 
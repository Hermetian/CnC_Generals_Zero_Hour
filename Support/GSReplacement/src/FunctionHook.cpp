#include "../include/FunctionHook.h"
#include "../include/GameBackend.h"
#include <stdexcept>

namespace GSReplacement {

// Static member initialization
std::map<void*, void*> FunctionHook::s_hookMap;
std::map<void*, OriginalFunction> FunctionHook::s_origFuncMap;

bool FunctionHook::Initialize() {
    // Nothing specific to initialize
    Logger::Info("FunctionHook system initialized");
    return true;
}

void FunctionHook::Shutdown() {
    // Unhook all functions
    auto it = s_origFuncMap.begin();
    while (it != s_origFuncMap.end()) {
        void* targetFunc = it->first;
        it++; // Increment iterator before unhooking to avoid invalidating it
        UnhookFunction(targetFunc);
    }
    
    Logger::Info("FunctionHook system shutdown. All hooks removed.");
}

bool FunctionHook::HookFunction(void* targetFunc, void* hookFunc, const std::string& funcName) {
    if (!targetFunc || !hookFunc) {
        Logger::Error("HookFunction: Invalid function pointer(s)");
        return false;
    }
    
    // Check if already hooked
    if (s_origFuncMap.find(targetFunc) != s_origFuncMap.end()) {
        Logger::Warning("Function already hooked: " + funcName);
        return false;
    }
    
    OriginalFunction origFunc;
    origFunc.address = targetFunc;
    
    // Write jump instruction
    if (!WriteJumpInstruction(targetFunc, hookFunc, origFunc)) {
        Logger::Error("Failed to write jump instruction for: " + funcName);
        return false;
    }
    
    // Save original function info
    s_origFuncMap[targetFunc] = origFunc;
    s_hookMap[targetFunc] = hookFunc;
    
    Logger::Info("Successfully hooked function: " + funcName);
    return true;
}

bool FunctionHook::HookFunction(const std::string& dllName, const std::string& exportName, void* hookFunc) {
    HMODULE hModule = GetModuleHandleA(dllName.c_str());
    if (!hModule) {
        // Try to load the DLL if it's not already loaded
        hModule = LoadLibraryA(dllName.c_str());
        if (!hModule) {
            Logger::Error("Failed to load DLL: " + dllName);
            return false;
        }
    }
    
    void* targetFunc = GetProcAddress(hModule, exportName.c_str());
    if (!targetFunc) {
        Logger::Error("Failed to find export: " + exportName + " in " + dllName);
        return false;
    }
    
    return HookFunction(targetFunc, hookFunc, dllName + "::" + exportName);
}

bool FunctionHook::UnhookFunction(void* targetFunc) {
    auto it = s_origFuncMap.find(targetFunc);
    if (it == s_origFuncMap.end()) {
        Logger::Warning("Function not hooked, can't unhook");
        return false;
    }
    
    const OriginalFunction& origFunc = it->second;
    
    // Restore original bytes
    if (!RestoreOriginalBytes(targetFunc, origFunc)) {
        Logger::Error("Failed to restore original bytes");
        return false;
    }
    
    // Remove from maps
    s_origFuncMap.erase(it);
    s_hookMap.erase(targetFunc);
    
    Logger::Info("Successfully unhooked function");
    return true;
}

bool FunctionHook::WriteJumpInstruction(void* targetAddress, void* jumpTo, OriginalFunction& origFunc) {
    DWORD oldProtect;
    
    // Make memory writable
    if (!MakeMemoryWritable(targetAddress, 10, oldProtect)) {
        return false;
    }
    
    // Save original bytes before overwriting
    memcpy(origFunc.originalBytes, targetAddress, 10);
    origFunc.bytesOverwritten = 5; // Size of jmp instruction
    
    // Write jmp instruction (E9 + relative offset)
    unsigned char* target = static_cast<unsigned char*>(targetAddress);
    target[0] = 0xE9; // JMP opcode
    
    // Calculate relative address (jumpTo - targetAddress - 5)
    // The -5 accounts for the size of the JMP instruction itself
    DWORD relativeAddress = (DWORD)jumpTo - (DWORD)targetAddress - 5;
    
    // Write the relative address (little-endian)
    *((DWORD*)(target + 1)) = relativeAddress;
    
    // Restore memory protection
    RestoreMemoryProtection(targetAddress, 10, oldProtect);
    
    return true;
}

bool FunctionHook::RestoreOriginalBytes(void* targetAddress, const OriginalFunction& origFunc) {
    DWORD oldProtect;
    
    // Make memory writable
    if (!MakeMemoryWritable(targetAddress, origFunc.bytesOverwritten, oldProtect)) {
        return false;
    }
    
    // Restore original bytes
    memcpy(targetAddress, origFunc.originalBytes, origFunc.bytesOverwritten);
    
    // Restore memory protection
    RestoreMemoryProtection(targetAddress, origFunc.bytesOverwritten, oldProtect);
    
    return true;
}

bool FunctionHook::MakeMemoryWritable(void* address, size_t size, DWORD& oldProtect) {
    return VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect) != 0;
}

bool FunctionHook::RestoreMemoryProtection(void* address, size_t size, DWORD oldProtect) {
    DWORD dummy;
    return VirtualProtect(address, size, oldProtect, &dummy) != 0;
}

} // namespace GSReplacement 
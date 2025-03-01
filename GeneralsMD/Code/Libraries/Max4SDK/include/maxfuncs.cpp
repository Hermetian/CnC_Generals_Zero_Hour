#include "Max.h"
#include "stdmat.h"
#include "modstack.h"

// Global interface instance
static Interface gInterface;

// Get the global interface
Interface* GetCOREInterface() {
    return &gInterface;
}

// Get ticks per frame (default to 30fps)
TimeValue GetTicksPerFrame() {
    return 4800 / 30;  // 4800 ticks per second at 30fps
}

// Helper to get a TriObject from a node
TriObject* GetTriObjectFromNode(INode* node, TimeValue t) {
    if (!node) return nullptr;
    
    Object* obj = node->GetObjectRef();
    if (!obj) return nullptr;
    
    // Try to convert to TriObject
    return obj->ConvertToType(t, 0);  // 0 would normally be a TriObject class ID
}

// Release a TriObject that was created by GetTriObjectFromNode
void ReleaseTriObject(TriObject* tri) {
    // In a real implementation, this would delete the object if temporary
    // For this stub, do nothing
}

// Create a derived object
IDerivedObject* CreateDerivedObject(Object* obj) {
    return new IDerivedObject(obj);
}

// Find a modifier in an object's modifier stack
int FindModifier(Object* obj, Modifier* mod) {
    if (!obj || !mod) return -1;
    
    // Check if it's a derived object
    IDerivedObject* dobj = dynamic_cast<IDerivedObject*>(obj);
    if (!dobj) return -1;
    
    // Look for the modifier
    int numMods = dobj->NumModifiers();
    for (int i = 0; i < numMods; i++) {
        if (dobj->GetModifier(i) == mod) {
            return i;
        }
    }
    
    return -1;
} 
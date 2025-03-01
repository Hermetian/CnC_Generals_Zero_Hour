#ifndef MODSTACK_H
#define MODSTACK_H

#include "Max.h"

// Forward declarations
class IDerivedObject;
class ModContext;

// Modifier - Base class for object modifiers
class Modifier {
protected:
    std::string name;
    
public:
    Modifier() {}
    virtual ~Modifier() {}
    
    void SetName(const char* n) { name = n; }
    const char* GetName() const { return name.c_str(); }
    
    // Apply the modifier to an object
    virtual void ModifyObject(TimeValue t, Object* obj, ModContext& mc) {}
    
    // Get/Set parameters
    virtual int NumParams() const { return 0; }
    virtual float GetParam(int i, TimeValue t) const { return 0.0f; }
    virtual void SetParam(int i, TimeValue t, float val) {}
};

// Derived Object - Object with modifiers applied
class IDerivedObject : public Object {
protected:
    Object* baseObject;
    std::vector<Modifier*> modifiers;
    std::vector<ModContext*> contexts;
    
public:
    IDerivedObject(Object* obj = nullptr) : baseObject(obj) {}
    virtual ~IDerivedObject() {}
    
    // Modifier methods
    int NumModifiers() const { return modifiers.size(); }
    Modifier* GetModifier(int i) const { 
        if (i >= 0 && i < (int)modifiers.size()) {
            return modifiers[i];
        }
        return nullptr;
    }
    
    void AddModifier(Modifier* mod, ModContext* mc = nullptr) {
        modifiers.push_back(mod);
        contexts.push_back(mc);
    }
    
    void DeleteModifier(int i) {
        if (i >= 0 && i < (int)modifiers.size()) {
            // In real implementation, would delete the modifier
            modifiers.erase(modifiers.begin() + i);
            contexts.erase(contexts.begin() + i);
        }
    }
    
    Object* GetBaseObject() const { return baseObject; }
    void SetBaseObject(Object* obj) { baseObject = obj; }
};

// Modifier context - Provides information to modifiers
class ModContext {
public:
    Box3 box;     // Bounding box
    Matrix3 tm;   // Transformation matrix
    
    ModContext() {}
    ModContext(const Box3& b, const Matrix3& m) : box(b), tm(m) {}
};

// Helper functions
extern IDerivedObject* CreateDerivedObject(Object* obj);
extern int FindModifier(Object* obj, Modifier* mod);

#endif // MODSTACK_H 
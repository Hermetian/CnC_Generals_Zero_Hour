#ifndef STDMAT_H
#define STDMAT_H

#include "Max.h"

// Standard material parameter IDs
#define STDMAT_AMBIENT        0
#define STDMAT_DIFFUSE        1
#define STDMAT_SPECULAR       2
#define STDMAT_SHININESS      3
#define STDMAT_SHIN_STR       4
#define STDMAT_OPACITY        5
#define STDMAT_SELF_ILLUM     6
#define STDMAT_TEXTURE        7
#define STDMAT_TEXTURE2       8
#define STDMAT_TEXTUREMASK    9

// Texmap types
#define TEXMAP_DIFFUSE        0
#define TEXMAP_SPECULAR       1
#define TEXMAP_OPACITY        2
#define TEXMAP_BUMP           3
#define TEXMAP_REFLECTION     4
#define TEXMAP_REFRACTION     5

// Forward declarations for classes not fully defined in Max.h
class BitmapTex;

// Standard material extension of Mtl class
class StdMat2 : public StdMat {
private:
    Texmap* texmaps[10]; // Array of texmaps
    bool texmapOn[10];   // Whether each texmap is enabled
    
public:
    StdMat2() {
        for (int i = 0; i < 10; i++) {
            texmaps[i] = nullptr;
            texmapOn[i] = false;
        }
    }
    
    virtual ~StdMat2() {
        // In a real implementation, would delete texmaps
    }
    
    // Texmap methods
    void SetTexmap(int slot, Texmap* tex) {
        if (slot >= 0 && slot < 10) {
            texmaps[slot] = tex;
        }
    }
    
    Texmap* GetTexmap(int slot) const {
        if (slot >= 0 && slot < 10) {
            return texmaps[slot];
        }
        return nullptr;
    }
    
    void SetTexmapOn(int slot, bool on) {
        if (slot >= 0 && slot < 10) {
            texmapOn[slot] = on;
        }
    }
    
    bool GetTexmapOn(int slot) const {
        if (slot >= 0 && slot < 10) {
            return texmapOn[slot];
        }
        return false;
    }
};

// BitmapTex - Texture from a bitmap image
class BitmapTex : public Texmap {
private:
    std::string fileName;
    UVGen* uvGen;
    
public:
    BitmapTex() : uvGen(nullptr) {}
    virtual ~BitmapTex() {}
    
    void SetFileName(const char* name) { fileName = name; }
    const char* GetFileName() const { return fileName.c_str(); }
    
    UVGen* GetUVGen() const { return uvGen; }
    void SetUVGen(UVGen* gen) { uvGen = gen; }
};

#endif // STDMAT_H 
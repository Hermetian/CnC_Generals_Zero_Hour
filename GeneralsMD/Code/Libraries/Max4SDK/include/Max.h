#ifndef MAX_H
#define MAX_H

#include <cmath>
#include <string>
#include <vector>

// Basic Max SDK type definitions for 3DS Max 4
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef char* TCHAR;
typedef void* HWND;
typedef void* HINSTANCE;
typedef int BOOL;

// Forward declarations
class Interface;
class INode;
class Object;
class Modifier;
class TriObject;
class Mesh;
class Material;
class Mtl;
class StdMat;
class Texmap;
class UVGen;
class XYZGen;
class Matrix3;
class Point3;
class Point2;
class Point4;
class Color;
class TimeValue;
class BitArray;
class MeshNormalSpec;
class MeshDelta;
class TextureOutput;
class ModContext;
class Box3;

// Global variables
extern Interface* GetCOREInterface();
extern TimeValue GetTicksPerFrame();

// TimeValue - Time in MAX is stored as integer ticks
typedef int TimeValue;
const int FOREVER = 0x7fffffff;

// Color class - Simple RGBA color
class Color {
public:
    float r, g, b, a;
    
    Color() : r(0), g(0), b(0), a(1.0f) {}
    Color(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) {}
    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
};

// Point classes - 2D/3D/4D vectors
class Point2 {
public:
    float x, y;
    
    Point2() : x(0), y(0) {}
    Point2(float x, float y) : x(x), y(y) {}
    
    Point2 operator+(const Point2& b) const { return Point2(x + b.x, y + b.y); }
    Point2 operator-(const Point2& b) const { return Point2(x - b.x, y - b.y); }
    Point2 operator*(float s) const { return Point2(x * s, y * s); }
};

class Point3 {
public:
    float x, y, z;
    
    Point3() : x(0), y(0), z(0) {}
    Point3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    Point3 operator+(const Point3& b) const { return Point3(x + b.x, y + b.y, z + b.z); }
    Point3 operator-(const Point3& b) const { return Point3(x - b.x, y - b.y, z - b.z); }
    Point3 operator*(float s) const { return Point3(x * s, y * s, z * s); }
    float Length() const { return sqrt(x*x + y*y + z*z); }
    
    // Array access operator (used in the code)
    float& operator[](int i) { 
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }
    
    const float& operator[](int i) const { 
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }
};

class Point4 {
public:
    float x, y, z, w;
    
    Point4() : x(0), y(0), z(0), w(1.0f) {}
    Point4(float x, float y, float z, float w = 1.0f) : x(x), y(y), z(z), w(w) {}
};

// Matrix3 - 4x3 transformation matrix (used extensively in the code)
class Matrix3 {
public:
    Point3 rows[3];
    Point3 translation;
    
    Matrix3() {
        rows[0] = Point3(1,0,0);
        rows[1] = Point3(0,1,0);
        rows[2] = Point3(0,0,1);
        translation = Point3(0,0,0);
    }
    
    // Array access operator
    Point3& operator[](int i) { 
        if (i < 3) return rows[i];
        return translation;
    }
    
    const Point3& operator[](int i) const { 
        if (i < 3) return rows[i];
        return translation;
    }
    
    // Matrix operations
    Matrix3 Inverse() const { 
        // Stub implementation - would need proper matrix inversion
        Matrix3 m;
        return m; 
    }
    
    // Transform a point
    Point3 PointTransform(const Point3& p) const {
        return Point3(
            p.x * rows[0].x + p.y * rows[1].x + p.z * rows[2].x + translation.x,
            p.x * rows[0].y + p.y * rows[1].y + p.z * rows[2].y + translation.y,
            p.x * rows[0].z + p.y * rows[1].z + p.z * rows[2].z + translation.z
        );
    }
};

// Box3 - Bounding box
class Box3 {
public:
    Point3 pmin, pmax;
    
    Box3() {}
    Box3(const Point3& min, const Point3& max) : pmin(min), pmax(max) {}
};

// BitArray - Bit array for flags
class BitArray {
private:
    std::vector<DWORD> bits;
    int numBits;
    
public:
    BitArray() : numBits(0) {}
    BitArray(int size) : numBits(size) {
        bits.resize((size + 31) / 32, 0);
    }
    
    void SetSize(int size) {
        numBits = size;
        bits.resize((size + 31) / 32, 0);
    }
    
    void Set(int i) {
        if (i >= 0 && i < numBits) {
            bits[i / 32] |= (1 << (i % 32));
        }
    }
    
    void Clear(int i) {
        if (i >= 0 && i < numBits) {
            bits[i / 32] &= ~(1 << (i % 32));
        }
    }
    
    bool operator[](int i) const {
        if (i >= 0 && i < numBits) {
            return (bits[i / 32] & (1 << (i % 32))) != 0;
        }
        return false;
    }
};

// Mesh class - Represents a mesh in MAX
class Mesh {
public:
    // Vertex data
    std::vector<Point3> verts;
    std::vector<Point3> normals;
    std::vector<Point2> tverts;  // Texture vertices
    
    // Face data
    struct Face {
        int v[3];     // Vertex indices
        int matID;    // Material ID
        int smGroup;  // Smoothing group
    };
    std::vector<Face> faces;
    
    // Constructor
    Mesh() {}
    
    // Methods
    int getNumVerts() const { return verts.size(); }
    int getNumFaces() const { return faces.size(); }
    int getNumTVerts() const { return tverts.size(); }
    
    void setNumVerts(int n) { verts.resize(n); }
    void setNumFaces(int n) { faces.resize(n); }
    void setNumTVerts(int n) { tverts.resize(n); }
    
    Point3& getVert(int i) { return verts[i]; }
    Face& getFace(int i) { return faces[i]; }
    
    void buildNormals() {
        // Stub implementation - would calculate normals
        normals.resize(verts.size());
    }
};

// TriObject - Wrapper for mesh objects
class TriObject {
public:
    Mesh mesh;
    
    Mesh& GetMesh() { return mesh; }
    const Mesh& GetMesh() const { return mesh; }
};

// Object - Base class for all objects
class Object {
public:
    virtual ~Object() {}
    
    virtual int NumPoints() const { return 0; }
    virtual Point3 GetPoint(int i) const { return Point3(); }
    virtual void SetPoint(int i, const Point3& p) {}
    
    // Convert to different object types
    virtual TriObject* ConvertToType(TimeValue t, int type) { return nullptr; }
};

// INode - Interface to scene nodes
class INode {
private:
    std::string name;
    Matrix3 objectTM;
    Object* obj;
    INode* parent;
    std::vector<INode*> children;
    
public:
    INode() : obj(nullptr), parent(nullptr) {}
    virtual ~INode() {}
    
    // Name methods
    void SetName(const char* n) { name = n; }
    const char* GetName() const { return name.c_str(); }
    
    // Object methods
    void SetObjectRef(Object* o) { obj = o; }
    Object* GetObjectRef() { return obj; }
    
    // Transformation methods
    Matrix3 GetNodeTM(TimeValue t) const { return objectTM; }
    void SetNodeTM(TimeValue t, const Matrix3& tm) { objectTM = tm; }
    
    // Hierarchy methods
    INode* GetParentNode() const { return parent; }
    void SetParent(INode* p) { parent = p; }
    int NumChildren() const { return children.size(); }
    INode* GetChildNode(int i) const { return children[i]; }
    void AddChild(INode* child) { children.push_back(child); }
};

// Material classes
class Mtl {
protected:
    std::string name;
    
public:
    Mtl() {}
    virtual ~Mtl() {}
    
    void SetName(const char* n) { name = n; }
    const char* GetName() const { return name.c_str(); }
    
    virtual int NumSubMtls() const { return 0; }
    virtual Mtl* GetSubMtl(int i) const { return nullptr; }
};

class StdMat : public Mtl {
protected:
    Color ambient;
    Color diffuse;
    Color specular;
    float shininess;
    float opacity;
    
public:
    StdMat() : shininess(0), opacity(1.0f) {}
    
    void SetAmbient(const Color& c) { ambient = c; }
    void SetDiffuse(const Color& c) { diffuse = c; }
    void SetSpecular(const Color& c) { specular = c; }
    void SetShininess(float f) { shininess = f; }
    void SetOpacity(float f) { opacity = f; }
    
    Color GetAmbient() const { return ambient; }
    Color GetDiffuse() const { return diffuse; }
    Color GetSpecular() const { return specular; }
    float GetShininess() const { return shininess; }
    float GetOpacity() const { return opacity; }
};

// Texture related classes
class Texmap {
protected:
    std::string name;
    
public:
    Texmap() {}
    virtual ~Texmap() {}
    
    void SetName(const char* n) { name = n; }
    const char* GetName() const { return name.c_str(); }
};

class UVGen {
public:
    UVGen() {}
    virtual ~UVGen() {}
    
    virtual Point2 GetUVMapping(const Point3& p) const { return Point2(); }
};

class XYZGen {
public:
    XYZGen() {}
    virtual ~XYZGen() {}
    
    virtual Point3 GetXYZ(const Point3& p) const { return Point3(); }
};

// Interface - Main interface to MAX
class Interface {
public:
    Interface() {}
    virtual ~Interface() {}
    
    // Scene methods
    virtual INode* GetRootNode() { return nullptr; }
    virtual int GetNumMaterials() const { return 0; }
    virtual Mtl* GetMaterial(int i) const { return nullptr; }
};

// External helper functions that might be used
extern TriObject* GetTriObjectFromNode(INode* node, TimeValue t);
extern void ReleaseTriObject(TriObject* tri);

// Define some common constants used in MAX
#define FOREVER 0x7fffffff
#define VERTEX_TESS_COPLANAR 0.1f
#define TEXMAP_CHANNEL_0 0
#define TEXMAP_CHANNEL_1 1

#endif // MAX_H 
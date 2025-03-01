# 3DSMax 4 SDK Stub Implementation

This directory contains a minimal stub implementation of the 3DSMax 4 SDK to satisfy build dependencies for the Command & Conquer: Generals - Zero Hour codebase.

## Purpose

This stub implementation provides:
- Header files with necessary class and function declarations
- Basic data structures (Matrix3, Point3, Mesh, etc.)
- Empty/minimal implementations of required functions

## Limitations

This is **NOT** a full implementation of the 3DSMax SDK. It is designed only to:
1. Allow the code to compile
2. Provide minimal structure for the build process
3. Support basic functionality for the WW3D export tools

The implementation has the following limitations:
- Most functions have minimal or no actual implementation
- No plugin support is provided
- No UI components are implemented
- No actual 3DSMax runtime functionality is provided

## Usage

This implementation should be used only for:
- Building the codebase when a full 3DSMax SDK is not available
- Understanding the structure and dependencies of the export tools
- Testing basic functionality

## Included Components

The current implementation includes:
- `Max.h` - Core data structures and definitions
- `stdmat.h` - Standard material classes and definitions
- `modstack.h` - Modifier stack and related classes

## Extending

If additional functionality is needed:
1. Identify the specific headers or functions required
2. Create stub implementations with minimal functionality
3. Update the existing headers as needed

## Obtaining the Full SDK

For full functionality, you will need the actual 3DSMax 4 SDK from Autodesk. This can be obtained through:
- Autodesk Developer Network (for newer versions)
- Legacy SDK archives (if available)
- 3DSMax 4 installation media

## Compatibility Notes

This stub implementation is intended only for Command & Conquer: Generals - Zero Hour and may not be suitable for other projects that require the 3DSMax SDK. 
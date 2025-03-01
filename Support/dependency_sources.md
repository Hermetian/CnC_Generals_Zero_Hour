# Command & Conquer: Generals - Zero Hour Development Dependencies

This document provides download sources and alternatives for all dependencies required for C&C Generals Zero Hour development.

## Downloaded Dependencies

The following dependencies have been successfully downloaded to the repository:

1. **ZLib (1.1.4)** - Downloaded to `CnC_Generals_Zero_Hour/GeneralsMD/Code/Libraries/Source/Compression/ZLib/`
2. **LZH-Light (via lhasa)** - Downloaded to:
   - `CnC_Generals_Zero_Hour/GeneralsMD/Code/Libraries/Source/Compression/LZHCompress/CompLibSource/`
   - `CnC_Generals_Zero_Hour/GeneralsMD/Code/Libraries/Source/Compression/LZHCompress/CompLibHeader/`
3. **3DSMax 4 SDK (Stub Implementation)** - Created at `CnC_Generals_Zero_Hour/GeneralsMD/Code/Libraries/Max4SDK/`
   - Minimal implementation that includes core headers and data structures
   - Provides enough structure to compile the code without the full SDK
   - See `CnC_Generals_Zero_Hour/GeneralsMD/Code/Libraries/Max4SDK/README.md` for details

## Available Dependencies (Requiring Windows/Installation)

These dependencies are available but require Windows to install:

1. **DirectX SDK (Version 9.0)** - Downloaded to `temp_downloads/dx9sdk.exe`
   - This is the installer and would need to be run on Windows
   - After installation, files should be copied to `CnC_Generals_Zero_Hour/GeneralsMD/Code/Libraries/DirectX/`

## Dependencies with Download Challenges

1. **STLport (4.5.3)**
   - Original download links appear to be broken or corrupted
   - Alternative approach: Build from source using a more modern C++ standard library or use a compatibility layer

2. **BYTEmark**
   - Repository not found at expected location
   - Alternative: Use any modern benchmarking library or find an archive of the original

## Remaining Dependencies to Obtain

These dependencies still need to be acquired:

1. **NVASM**
   - This was NVIDIA's assembly shader toolkit
   - Modern alternative: Use HLSL/GLSL shaders with modern compilers
   - Place in: `CnC_Generals_Zero_Hour/GeneralsMD/Code/Tools/NVASM/`

2. **RAD Miles Sound System SDK**
   - Proprietary audio middleware
   - Requires license from RAD Game Tools (now part of Epic Games)
   - Place in: `CnC_Generals_Zero_Hour/GeneralsMD/Code/Libraries/Source/WWVegas/Miles6/`

3. **RAD Bink SDK**
   - Proprietary video middleware
   - Requires license from RAD Game Tools (now part of Epic Games)
   - Place in: `CnC_Generals_Zero_Hour/GeneralsMD/Code/GameEngineDevice/Include/VideoDevice/Bink`

4. **Miles Sound System "Asimp3"**
   - MP3 codec for Miles Sound System
   - Requires original files or Miles license
   - Place in: `CnC_Generals_Zero_Hour/GeneralsMD/Code/Libraries/WPAudio/Asimp3`

## Dependencies removed or replaced
1. **SafeDisk API**
   - Legacy copy protection system
   - No longer commercially available
   - All references removed in code
   - Place in: `CnC_Generals_Zero_Hour/GeneralsMD/Code/GameEngine/Include/Common/SafeDisk` and `CnC_Generals_Zero_Hour/GeneralsMD/Code/Tools/Launcher/SafeDisk/`

2. **GameSpy SDK**
   - Online services SDK (being replaced by your project)
   - Replaced by custom GameSpy conversion
   - Place in: `CnC_Generals_Zero_Hour/GeneralsMD/Code/Libraries/Source/GameSpy/`

## Notes on Proprietary Dependencies

Many of these dependencies are proprietary and require licenses or aren't publicly available anymore. For development purposes, you might need to:

1. Use placeholder/stub implementations (as we did with 3DSMax SDK)
2. Contact EA or the original vendors for license information
3. Consider modern alternatives where compatibility allows

## Alternative Approaches

Given the challenges with obtaining these legacy dependencies, consider these alternatives:

1. **Use a Pre-built Development Environment**: Look for community projects that have already collected these dependencies
2. **Create Stub Libraries**: Create minimal implementations that provide the same API but with limited functionality (demonstrated with 3DSMax SDK)
3. **Focus on GameSpy Replacement**: Since your primary goal is replacing the GameSpy functionality, you might not need all these dependencies
4. **Request the Dependencies from EA**: If this is an official or authorized project, consider requesting the original dependencies from EA

## Next Steps

1. For the GameSpy SDK specifically, continue with your replacement project as planned
2. For the remaining dependencies, prioritize based on what's most critical for your development goals
3. Consider creating stub/mock implementations for proprietary dependencies that cannot be easily obtained, similar to the approach taken with the 3DSMax SDK 
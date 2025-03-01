# Command & Conquer: Generals - Zero Hour Source Code Project

This repository contains the source code and development tools for Command & Conquer: Generals - Zero Hour, along with custom implementations for various dependencies.

## Project Overview

This project aims to provide:

1. **Source Code Access**: Access to the original Zero Hour game engine code for research and modification purposes.
2. **GameSpy Replacement**: A custom implementation of the GameSpy online services to restore multiplayer functionality.
3. **Build System**: Tools for building and replacing game components.
4. **Dependency Management**: Documentation and solutions for handling development dependencies.

## Repository Structure

- `/GSReplacement/` - GameSpy replacement implementation
- `/CnC_Generals_Zero_Hour/GeneralsMD/` - The Zero Hour game codebase (our primary focus)
- `/build_*.bat` - Build system scripts

## Build System

The repository includes a comprehensive build system for compiling and replacing game components:

- `build_config.bat` - Configuration settings
- `build_all.bat` - Full build process script 
- `replace_files.bat` - Targeted file replacement utility
- `BUILD_README.md` - Build system documentation

For detailed build instructions, see [BUILD_README.md](BUILD_README.md).

## Dependencies

The development environment requires several dependencies, some of which are proprietary and may need to be obtained separately. See [dependency_sources.md](dependency_sources.md) for a complete list of dependencies and their status.

## Getting Started

1. Install Command & Conquer: Generals - Zero Hour
2. Clone this repository
3. Run `setup_repo.bat` to set up the development environment
4. Configure `build_config.bat` with your game installation path
5. Run `build_all.bat` to build components
6. Launch the game to test your changes

## Current Status

- **GameSpy Replacement**: Functional, supports connecting to Revora or OpenSpy
- **3DSMax SDK**: Stub implementation available for building export tools
- **Core Game Code**: Source available but not all dependencies are resolved

## Legal Disclaimer

This project is not affiliated with or endorsed by Electronic Arts. All trademarks are the property of their respective owners. This repository is intended for educational and research purposes only. You must own a legitimate copy of Command & Conquer: Generals - Zero Hour to use this code.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request. 
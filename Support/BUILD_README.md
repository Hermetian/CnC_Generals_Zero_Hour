# Command & Conquer: Generals - Zero Hour Build System

This document provides instructions for building and replacing game files in the Command & Conquer: Generals - Zero Hour game.

## Overview

The build system consists of several scripts:

1. `build_config.bat` - Central configuration for paths and settings
2. `build_all.bat` - Main build script that builds all components and installs files
3. `replace_files.bat` - Utility for targeted file replacement

## Prerequisites

1. Original game installation of Command & Conquer: Generals - Zero Hour
2. Visual Studio 2019 or newer (for building C++ code)
3. CMake 3.10 or newer (for configuring builds)

## Setup

1. Edit `build_config.bat` and set the `GAME_INSTALL_PATH` to your game installation directory
2. Ensure all required development dependencies are installed (see `dependency_sources.md`)

## Building

### Full Build

To build all components and replace files in the game directory:

```
build_all.bat
```

This will:
1. Build the GameSpy replacement DLL
2. Place output files in the `build_output` directory
3. Replace configured files in the game installation

### Replacing Specific Files

To replace only specific files:

```
replace_files.bat [file_type]
```

Where `[file_type]` is one of:
- `gamespy` - Replace only the GameSpy DLL
- `core` - Replace core game files (not yet implemented)
- `engine` - Replace engine files (not yet implemented)
- `all` - Replace all configured files

## File Replacement Levels

The build system uses a tiered approach to file replacement:

### Level 1: Safe Replacements
- GameSpy replacement DLL
- Configuration files

### Level 2: Core Game Replacements
- Game data files (game.dat)
- User interface components

### Level 3: Engine Replacements
- Renderer DLL
- Audio DLL
- Physics/AI components

## Backups

By default, the system creates backups of original game files before replacing them. These are stored in the `game_backups` directory.

## Troubleshooting

### Common Issues

1. **File Not Found**: Ensure the game path is correctly set in `build_config.bat`
2. **Build Errors**: Check that all dependencies are correctly installed
3. **Game Crashes**: Try restoring original files from the backup directory

### Logs

Build logs are stored in the respective build directories.

## Adding New Components

To add a new component to the build system:

1. Create a build script for the component
2. Add it to the build process in `build_all.bat`
3. Update the replacement configuration in `build_config.bat`
4. Add handling logic in `replace_files.bat`

## Safety Measures

- Always test replacements with a clean game installation first
- Use the backup system to restore original files if needed
- Start with minimal replacements (GameSpy only) before trying more extensive replacements 
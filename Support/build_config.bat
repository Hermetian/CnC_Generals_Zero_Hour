@echo off
REM ====================================================================
REM BUILD CONFIGURATION FOR C&C GENERALS ZERO HOUR DEVELOPMENT
REM ====================================================================

REM === MAIN PATHS ===
REM Set this to your original game installation
set GAME_INSTALL_PATH=C:\Program Files (x86)\EA Games\Command & Conquer Generals Zero Hour
REM Check if the path exists
if not exist "%GAME_INSTALL_PATH%" (
    echo WARNING: Game installation path does not exist: %GAME_INSTALL_PATH%
    echo Please update the GAME_INSTALL_PATH in build_config.bat
)

REM Set this to your build output directory
set BUILD_OUTPUT_PATH=%~dp0\build_output

REM === SOURCE CODE PATHS ===
REM Zero Hour (GeneralsMD) is now the primary codebase
set SOURCE_ROOT=CnC_Generals_Zero_Hour\GeneralsMD
set CODE_ROOT=%SOURCE_ROOT%\Code
set LIBRARY_ROOT=%CODE_ROOT%\Libraries

REM === LIBRARY PATHS ===
set MAX4SDK_PATH=%LIBRARY_ROOT%\Max4SDK
set DIRECTX_PATH=%LIBRARY_ROOT%\DirectX
set STLPORT_PATH=%LIBRARY_ROOT%\STLport-4.5.3
set ZLIB_PATH=%LIBRARY_ROOT%\Source\Compression\ZLib
set LZH_PATH=%LIBRARY_ROOT%\Source\Compression\LZHCompress

REM === FILES TO REPLACE ===
REM Level 1: Safe replacements (only GameSpy)
set REPLACE_GAMESPY=1
set GAMESPY_DLL=gamespy.dll

REM Level 2: Core game replacements (uncomment when ready)
REM set REPLACE_CORE=1
REM set CORE_FILES=game.dat

REM Level 3: Engine replacements (uncomment when ready)
REM set REPLACE_ENGINE=1
REM set ENGINE_FILES=renderer.dll audio.dll

REM === BUILD CONFIGURATION ===
REM Set to Release or Debug
set BUILD_CONFIG=Release

REM === BACKUP SETTINGS ===
set CREATE_BACKUPS=1
set BACKUP_PATH=%~dp0\game_backups

echo Build configuration loaded.
echo Game path: %GAME_INSTALL_PATH%
echo Source root: %SOURCE_ROOT%
echo Build output: %BUILD_OUTPUT_PATH%
echo. 
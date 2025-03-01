@echo off
echo.
echo ====================================================================
echo FILE REPLACEMENT UTILITY
echo ====================================================================
echo.

REM Load configuration
call build_config.bat

REM Check arguments
if "%1"=="" (
    echo Usage: replace_files.bat [file_type] [specific_file]
    echo.
    echo Available file types:
    echo   gamespy  - Replace GameSpy DLL
    echo   core     - Replace core game files
    echo   engine   - Replace engine files
    echo   all      - Replace all configured files
    echo.
    echo Examples:
    echo   replace_files.bat gamespy      - Replace GameSpy DLL
    echo   replace_files.bat all          - Replace all configured files
    goto :end
)

REM Create necessary directories
if not exist "%BACKUP_PATH%" mkdir "%BACKUP_PATH%"

REM Check if game directory exists
if not exist "%GAME_INSTALL_PATH%" (
    echo ERROR: Game installation path does not exist: %GAME_INSTALL_PATH%
    echo Please update the GAME_INSTALL_PATH in build_config.bat
    goto :error
)

REM Process replacement based on argument
set FILE_TYPE=%1
set SPECIFIC_FILE=%2

REM GameSpy replacement
if "%FILE_TYPE%"=="gamespy" goto :replace_gamespy
if "%FILE_TYPE%"=="all" goto :replace_all

REM Core game replacements
if "%FILE_TYPE%"=="core" goto :replace_core

REM Engine replacements
if "%FILE_TYPE%"=="engine" goto :replace_engine

echo Unknown file type: %FILE_TYPE%
goto :usage

:replace_all
echo Replacing all configured files...
echo.

:replace_gamespy
if "%REPLACE_GAMESPY%"=="1" (
    echo Replacing GameSpy DLL: %GAMESPY_DLL%
    
    REM Create backup if needed
    if "%CREATE_BACKUPS%"=="1" (
        if exist "%GAME_INSTALL_PATH%\%GAMESPY_DLL%" (
            if not exist "%BACKUP_PATH%\original_%GAMESPY_DLL%" (
                echo   Creating backup of original %GAMESPY_DLL%...
                copy "%GAME_INSTALL_PATH%\%GAMESPY_DLL%" "%BACKUP_PATH%\original_%GAMESPY_DLL%"
            )
        )
    )
    
    REM Copy the file
    if exist "%BUILD_OUTPUT_PATH%\%GAMESPY_DLL%" (
        echo   Copying %GAMESPY_DLL% to game directory...
        copy "%BUILD_OUTPUT_PATH%\%GAMESPY_DLL%" "%GAME_INSTALL_PATH%\%GAMESPY_DLL%"
        echo   GameSpy DLL replaced successfully.
    ) else (
        echo   ERROR: %GAMESPY_DLL% not found in build output directory.
        echo   Make sure you've built the GameSpy replacement first.
    )
    echo.
)

if "%FILE_TYPE%"=="gamespy" goto :end
if "%FILE_TYPE%"=="all" goto :replace_core
goto :end

:replace_core
if "%REPLACE_CORE%"=="1" (
    echo Replacing core game files: %CORE_FILES%
    echo NOTE: Core game replacement is not yet implemented.
    echo.
)

if "%FILE_TYPE%"=="core" goto :end
if "%FILE_TYPE%"=="all" goto :replace_engine
goto :end

:replace_engine
if "%REPLACE_ENGINE%"=="1" (
    echo Replacing engine files: %ENGINE_FILES%
    echo NOTE: Engine replacement is not yet implemented.
    echo.
)

goto :end

:usage
echo.
echo Usage: replace_files.bat [file_type] [specific_file]
echo.
echo Available file types:
echo   gamespy  - Replace GameSpy DLL
echo   core     - Replace core game files
echo   engine   - Replace engine files
echo   all      - Replace all configured files
echo.
goto :end

:error
echo.
echo File replacement encountered errors. Please check the messages above.
echo.

:end 
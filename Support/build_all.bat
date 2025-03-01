@echo off
echo.
echo ====================================================================
echo COMMAND & CONQUER: GENERALS - ZERO HOUR BUILD SYSTEM
echo ====================================================================
echo.

REM Load configuration
call build_config.bat

REM Create necessary directories
if not exist "%BUILD_OUTPUT_PATH%" mkdir "%BUILD_OUTPUT_PATH%"
if not exist "%BACKUP_PATH%" mkdir "%BACKUP_PATH%"

REM ====================================================================
REM STEP 1: BUILD GAMESPY REPLACEMENT
REM ====================================================================
echo Building GameSpy Replacement...
echo.

cd GSReplacement
call build.bat
cd ..

REM Copy GameSpy DLL to build output
echo Copying GameSpy DLL to build output...
if exist "GSReplacement\build\Release\gamespy.dll" (
    copy "GSReplacement\build\Release\gamespy.dll" "%BUILD_OUTPUT_PATH%\gamespy.dll"
    echo GameSpy DLL successfully built and copied.
) else (
    echo ERROR: GameSpy DLL build failed or file not found.
    goto :error
)

REM ====================================================================
REM STEP 2: BUILD OTHER COMPONENTS (PLACEHOLDER)
REM ====================================================================
echo.
echo NOTE: Building other game components is not yet implemented.
echo Only the GameSpy replacement is currently built.
echo.
echo To build other components, add them to this build script when ready.
echo.

REM ====================================================================
REM STEP 3: INSTALL FILES TO GAME DIRECTORY
REM ====================================================================
echo.
echo Preparing to replace files in game directory...

REM Check if game directory exists
if not exist "%GAME_INSTALL_PATH%" (
    echo ERROR: Game installation path does not exist: %GAME_INSTALL_PATH%
    echo Please update the GAME_INSTALL_PATH in build_config.bat
    goto :error
)

echo.
echo The following replacements will be made:

REM GameSpy replacement
if "%REPLACE_GAMESPY%"=="1" (
    echo - GameSpy DLL: %GAMESPY_DLL%
    
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
    echo   Copying %GAMESPY_DLL% to game directory...
    copy "%BUILD_OUTPUT_PATH%\%GAMESPY_DLL%" "%GAME_INSTALL_PATH%\%GAMESPY_DLL%"
)

REM Core game replacements (currently commented out)
if "%REPLACE_CORE%"=="1" (
    echo - Core game files: %CORE_FILES%
    echo   NOTE: Core game replacement is not yet implemented.
)

REM Engine replacements (currently commented out)
if "%REPLACE_ENGINE%"=="1" (
    echo - Engine files: %ENGINE_FILES%
    echo   NOTE: Engine replacement is not yet implemented.
)

echo.
echo Build and replacement process completed successfully.
echo.
echo To run the game with your new builds:
echo 1. Launch the game normally through your game launcher
echo 2. The game will use your custom built DLL instead of the original
echo.
goto :end

:error
echo.
echo Build process encountered errors. Please check the messages above.
echo.

:end
pause 
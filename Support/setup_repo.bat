@echo off
echo.
echo ====================================================================
echo COMMAND & CONQUER: GENERALS - ZERO HOUR REPOSITORY SETUP
echo ====================================================================
echo.

REM Create build output directory
if not exist "build_output" (
    echo Creating build output directory...
    mkdir "build_output"
)

REM Create backups directory
if not exist "game_backups" (
    echo Creating game backups directory...
    mkdir "game_backups"
)

REM Create temporary downloads directory
if not exist "temp_downloads" (
    echo Creating temporary downloads directory...
    mkdir "temp_downloads"
)

REM Check if library migration is needed
echo Checking repository structure...
if exist "Code\Libraries" (
    echo.
    echo Library migration is needed to consolidate on Zero Hour (GeneralsMD) structure.
    echo.
    set /p MIGRATE="Do you want to migrate libraries now? (Y/N): "
    if /i "%MIGRATE%"=="Y" (
        echo.
        echo Running library migration...
        call migrate_libraries.bat
    ) else (
        echo.
        echo Library migration skipped. You can run migrate_libraries.bat manually later.
    )
)

REM Configure the build settings
echo.
echo Please configure your game installation path.
echo Default is: C:\Program Files (x86)\EA Games\Command & Conquer Generals Zero Hour
echo.

set /p GAME_PATH="Enter your game installation path (or press Enter for default): "
if "%GAME_PATH%"=="" set GAME_PATH=C:\Program Files (x86)\EA Games\Command & Conquer Generals Zero Hour

REM Update the build_config.bat file
echo Updating build configuration...
powershell -Command "(Get-Content build_config.bat) -replace 'set GAME_INSTALL_PATH=.*', 'set GAME_INSTALL_PATH=%GAME_PATH%' | Set-Content build_config.bat"

echo.
echo Repository setup complete!
echo.
echo Next steps:
echo 1. Review the README.md file for an overview of the project
echo 2. Check the dependency_sources.md file for information about dependencies
echo 3. Run build_all.bat to build the GameSpy replacement DLL
echo.

pause 
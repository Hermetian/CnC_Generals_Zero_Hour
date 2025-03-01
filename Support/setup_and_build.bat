@echo off
echo.
echo ====================================================================
echo COMMAND & CONQUER: GENERALS - ZERO HOUR SETUP AND BUILD
echo ====================================================================
echo.

REM Set paths
set REPO_ROOT=%~dp0..
set SUPPORT_DIR=%REPO_ROOT%\Support
set GENERALSMD_DIR=%REPO_ROOT%\GeneralsMD
set CODE_DIR=%GENERALSMD_DIR%\Code
set TEMP_DIR=%REPO_ROOT%\temp_downloads

REM Create temp directory if it doesn't exist
if not exist "%TEMP_DIR%" mkdir "%TEMP_DIR%"

echo Welcome to the GeneralsMD setup and build script!
echo.
echo This script will help you set up the required dependencies and build GeneralsMD.
echo.
echo The following steps will be performed:
echo 1. Set up STLport-4.5.3
echo 2. Set up DirectX SDK
echo 3. Build GeneralsMD using Visual Studio 2022
echo.
set /p CONTINUE="Do you want to continue? (Y/N): "
if /i not "%CONTINUE%"=="Y" goto :end

echo.
echo ====================================================================
echo STEP 1: SETTING UP STLPORT-4.5.3
echo ====================================================================
echo.

set /p SETUP_STLPORT="Do you want to set up STLport-4.5.3? (Y/N): "
if /i "%SETUP_STLPORT%"=="Y" (
    echo Running STLport setup script...
    call "%SUPPORT_DIR%\setup_stlport.bat"
)

echo.
echo ====================================================================
echo STEP 2: SETTING UP DIRECTX SDK
echo ====================================================================
echo.

set /p SETUP_DIRECTX="Do you want to set up DirectX SDK? (Y/N): "
if /i "%SETUP_DIRECTX%"=="Y" (
    echo Running DirectX SDK setup script...
    call "%SUPPORT_DIR%\setup_directx.bat"
)

echo.
echo ====================================================================
echo STEP 3: BUILDING GENERALSMD
echo ====================================================================
echo.

set /p BUILD_GENERALSMD="Do you want to build GeneralsMD? (Y/N): "
if /i "%BUILD_GENERALSMD%"=="Y" (
    echo Running build script...
    call "%SUPPORT_DIR%\build_all.bat"
)

echo.
echo ====================================================================
echo SETUP AND BUILD PROCESS COMPLETED
echo ====================================================================
echo.
echo Thank you for using the GeneralsMD setup and build script!
echo.
echo If you encountered any issues, please check the error messages and try again.
echo.
echo For more information, please refer to the README.md file.
echo.

:end
pause 
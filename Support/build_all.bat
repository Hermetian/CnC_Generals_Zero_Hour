@echo off
echo.
echo ====================================================================
echo COMMAND & CONQUER: GENERALS - ZERO HOUR BUILD SYSTEM
echo ====================================================================
echo.

REM Set paths
set REPO_ROOT=%~dp0..
set GENERALSMD_DIR=%REPO_ROOT%\GeneralsMD
set CODE_DIR=%GENERALSMD_DIR%\Code
set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe"

echo Checking for Visual Studio 2022...
if not exist %VS_PATH% (
    echo ERROR: Visual Studio 2022 not found at %VS_PATH%
    echo Please install Visual Studio 2022 or update the path in this script.
    goto :error
)

echo.
echo Step 1: Checking for required dependencies
echo ----------------------------------------
echo.

REM Check for STLport
if not exist "%CODE_DIR%\Libraries\Source\STLport-4.5.3\stlport" (
    echo WARNING: STLport-4.5.3 not found.
    echo You need to download STLport-4.5.3 and extract it to:
    echo %CODE_DIR%\Libraries\Source\STLport-4.5.3\
    echo.
    echo Then apply the patch from %REPO_ROOT%\stlport.diff
    echo.
    set /p CONTINUE="Do you want to continue anyway? (Y/N): "
    if /i not "%CONTINUE%"=="Y" goto :error
)

REM Check for DirectX SDK
if not exist "%CODE_DIR%\Libraries\DirectX" (
    echo WARNING: DirectX SDK not found.
    echo You need to install DirectX SDK and copy the files to:
    echo %CODE_DIR%\Libraries\DirectX\
    echo.
    set /p CONTINUE="Do you want to continue anyway? (Y/N): "
    if /i not "%CONTINUE%"=="Y" goto :error
)

echo.
echo Step 2: Opening project in Visual Studio 2022
echo ----------------------------------------
echo.

echo Opening RTS.dsw in Visual Studio 2022...
echo This will convert the project to a modern format.
echo.
echo IMPORTANT: When prompted, select "Yes to All" to convert all projects.
echo.
echo After conversion, use "Build -> Batch Build" and select "Rebuild All"
echo.
set /p OPEN_VS="Open Visual Studio now? (Y/N): "
if /i "%OPEN_VS%"=="Y" (
    echo Starting Visual Studio...
    start "" %VS_PATH% "%CODE_DIR%\RTS.dsw"
)

echo.
echo Build script completed!
echo.
echo Next steps:
echo 1. Complete the build in Visual Studio
echo 2. Check the output in %GENERALSMD_DIR%\Run\
echo.
goto :end

:error
echo.
echo Build process failed or was cancelled.
echo.

:end
pause 
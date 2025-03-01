@echo off
echo.
echo ====================================================================
echo SETTING UP DIRECTX SDK
echo ====================================================================
echo.

REM Set paths
set REPO_ROOT=%~dp0..
set GENERALSMD_DIR=%REPO_ROOT%\GeneralsMD
set CODE_DIR=%GENERALSMD_DIR%\Code
set DIRECTX_DIR=%CODE_DIR%\Libraries\DirectX
set TEMP_DIR=%REPO_ROOT%\temp_downloads
set DIRECTX_EXE=%TEMP_DIR%\DXSDK_Jun10.exe
set DIRECTX_URL=https://download.microsoft.com/download/A/E/7/AE743F1F-632B-4809-87A9-AA1BB3458E31/DXSDK_Jun10.exe

REM Create temp directory if it doesn't exist
if not exist "%TEMP_DIR%" mkdir "%TEMP_DIR%"

echo Step 1: Downloading DirectX SDK (June 2010)
echo ----------------------------------------
echo.

echo Downloading DirectX SDK from Microsoft...
echo URL: %DIRECTX_URL%
echo.
echo This is a large download (approximately 570MB) and may take some time.
echo.

set /p DOWNLOAD="Do you want to download the DirectX SDK now? (Y/N): "
if /i not "%DOWNLOAD%"=="Y" goto :skip_download

powershell -Command "& {[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri '%DIRECTX_URL%' -OutFile '%DIRECTX_EXE%'}"

if not exist "%DIRECTX_EXE%" (
    echo ERROR: Failed to download DirectX SDK.
    goto :error
)

echo Download completed successfully.
echo.

:skip_download

echo Step 2: Installing DirectX SDK
echo ----------------------------------------
echo.

echo The DirectX SDK needs to be installed on your system.
echo.

if exist "%DIRECTX_EXE%" (
    set /p INSTALL="Do you want to install the DirectX SDK now? (Y/N): "
    if /i "%INSTALL%"=="Y" (
        echo Running DirectX SDK installer...
        echo Please follow the installation prompts.
        echo.
        start "" "%DIRECTX_EXE%"
        echo.
        echo After installation completes, please continue with this script.
        echo.
        pause
    )
) else (
    echo DirectX SDK installer not found at %DIRECTX_EXE%
    echo Please download and install the DirectX SDK manually from:
    echo https://www.microsoft.com/en-us/download/details.aspx?id=6812
    echo.
    set /p CONTINUE="Press Enter to continue..."
)

echo Step 3: Copying DirectX SDK files
echo ----------------------------------------
echo.

echo The DirectX SDK files need to be copied to the project directory.
echo.

set /p DXSDK_PATH="Enter the path to your DirectX SDK installation (e.g., C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)): "

if not exist "%DXSDK_PATH%" (
    echo ERROR: DirectX SDK path not found: %DXSDK_PATH%
    goto :error
)

echo Creating DirectX directory in the project...
if not exist "%DIRECTX_DIR%" mkdir "%DIRECTX_DIR%"
if not exist "%DIRECTX_DIR%\Include" mkdir "%DIRECTX_DIR%\Include"
if not exist "%DIRECTX_DIR%\Lib" mkdir "%DIRECTX_DIR%\Lib"

echo Copying DirectX SDK Include files...
xcopy /E /I /Y "%DXSDK_PATH%\Include\*" "%DIRECTX_DIR%\Include\"

echo Copying DirectX SDK Library files...
xcopy /E /I /Y "%DXSDK_PATH%\Lib\x86\*" "%DIRECTX_DIR%\Lib\"

echo.
echo DirectX SDK setup completed!
echo.
echo Next steps:
echo 1. Run build_all.bat to build the project
echo 2. Check for any compilation errors related to DirectX
echo.
goto :end

:error
echo.
echo DirectX SDK setup failed.
echo.

:end
pause 
@echo off
setlocal enabledelayedexpansion

echo.
echo ====================================================================
echo DIRECTX SETUP FOR C&C GENERALS
echo ====================================================================
echo.

REM Set paths
set "REPO_ROOT=%~dp0.."
set "GENERALSMD_DIR=%REPO_ROOT%\GeneralsMD"
set "CODE_DIR=%GENERALSMD_DIR%\Code"
set "DIRECTX_DIR=%CODE_DIR%\Libraries\DirectX"
set "TEMP_DIR=%REPO_ROOT%\temp_downloads"
set "DIRECTX_EXE=%TEMP_DIR%\DXSDK_Jun10.exe"
set "DIRECTX_URL=https://download.microsoft.com/download/A/E/7/AE743F1F-632B-4809-87A9-AA1BB3458E31/DXSDK_Jun10.exe"

echo REPO_ROOT=%REPO_ROOT%
echo DIRECTX_DIR=%DIRECTX_DIR%

REM Create directories if they don't exist
if not exist "%TEMP_DIR%" mkdir "%TEMP_DIR%"
if not exist "%DIRECTX_DIR%" mkdir "%DIRECTX_DIR%"
if not exist "%DIRECTX_DIR%\Include" mkdir "%DIRECTX_DIR%\Include"
if not exist "%DIRECTX_DIR%\Lib" mkdir "%DIRECTX_DIR%\Lib"

echo Phase 1: Checking for Windows SDK...
set SDK_FOUND=0
set "SDK_ROOT="

REM Check common Windows SDK locations
if exist "C:\Program Files (x86)\Windows Kits\10" (
    set "SDK_ROOT=C:\Program Files (x86)\Windows Kits\10"
    set SDK_FOUND=1
) else if exist "C:\Program Files\Windows Kits\10" (
    set "SDK_ROOT=C:\Program Files\Windows Kits\10"
    set SDK_FOUND=1
)

echo SDK_FOUND=%SDK_FOUND%
if %SDK_FOUND%==1 echo SDK_ROOT=%SDK_ROOT%

if %SDK_FOUND%==1 (
    echo Windows SDK found. Looking for latest version...
    
    REM Find the latest SDK version
    set "SDK_VERSION="
    for /f "delims=" %%i in ('dir /b /ad "%SDK_ROOT%\Include\10.*" 2^>nul') do (
        set "SDK_VERSION=%%i"
    )
    
    if not "!SDK_VERSION!"=="" (
        echo Found SDK version: !SDK_VERSION!
        set "SDK_INCLUDE=%SDK_ROOT%\Include\!SDK_VERSION!\um"
        set "SDK_LIB=%SDK_ROOT%\Lib\!SDK_VERSION!\um\x86"
        
        if not exist "!SDK_INCLUDE!" (
            echo SDK Include directory not found: !SDK_INCLUDE!
            goto use_directx_sdk
        )
        
        if not exist "!SDK_LIB!" (
            echo SDK Lib directory not found: !SDK_LIB!
            goto use_directx_sdk
        )
        
        echo Copying files from Windows SDK...
        echo.
        
        REM Copy DirectX 9 files from Windows SDK
        echo Copying DirectX 9 headers...
        for %%f in (d3d9.h d3d9caps.h d3d9types.h dinput.h dsound.h dxguid.h dxfile.h) do (
            echo   Copying %%f
            copy /y "!SDK_INCLUDE!\%%f" "%DIRECTX_DIR%\Include\" >nul 2>&1
            if errorlevel 1 echo   - Failed to copy %%f
        )
        
        echo Copying DirectX 9 libraries...
        for %%f in (d3d9.lib dinput8.lib dsound.lib dxguid.lib) do (
            echo   Copying %%f
            copy /y "!SDK_LIB!\%%f" "%DIRECTX_DIR%\Lib\" >nul 2>&1
            if errorlevel 1 echo   - Failed to copy %%f
        )
        
        REM Check for DirectX 8 components
        echo Checking for DirectX 8 components in Windows SDK...
        set DX8_FOUND=0
        if exist "!SDK_INCLUDE!\d3d8.h" if exist "!SDK_LIB!\d3d8.lib" set DX8_FOUND=1
        
        echo DX8_FOUND=!DX8_FOUND!
        
        if !DX8_FOUND!==1 (
            echo Found DirectX 8 components. Copying...
            
            for %%f in (d3d8.h d3d8caps.h d3d8types.h) do (
                echo   Copying %%f
                copy /y "!SDK_INCLUDE!\%%f" "%DIRECTX_DIR%\Include\" >nul 2>&1
                if errorlevel 1 echo   - Failed to copy %%f
            )
            
            echo   Copying d3d8.lib
            copy /y "!SDK_LIB!\d3d8.lib" "%DIRECTX_DIR%\Lib\" >nul 2>&1
            if errorlevel 1 echo   - Failed to copy d3d8.lib
            
            echo.
            echo Windows SDK setup completed.
            goto finished
        ) else (
            echo DirectX 8 components not found in Windows SDK.
            echo Modern Windows SDKs don't include DirectX 8.
            goto use_directx_sdk
        )
    ) else (
        echo Could not determine Windows SDK version.
        goto use_directx_sdk
    )
) else (
    echo Windows SDK not found.
    goto use_directx_sdk
)

:use_directx_sdk
echo.
echo Phase 2: Using DirectX SDK (June 2010)
echo ====================================================================
echo.
echo The DirectX SDK (June 2010) is required for DirectX 8 components.
echo.

REM Check for existing DX SDK
set "DXSDK_DIR="
if defined DXSDK_DIR (
    if exist "%DXSDK_DIR%" (
        echo Found DirectX SDK environment variable: %DXSDK_DIR%
        set "DXSDK_PATH=%DXSDK_DIR%"
        goto copy_directx_files
    )
)

REM Check common install locations
if exist "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)" (
    echo Found DirectX SDK at default location.
    set "DXSDK_PATH=C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)"
    goto copy_directx_files
) else if exist "C:\Program Files\Microsoft DirectX SDK (June 2010)" (
    echo Found DirectX SDK at default location.
    set "DXSDK_PATH=C:\Program Files\Microsoft DirectX SDK (June 2010)"
    goto copy_directx_files
)

REM Need to download or specify DirectX SDK path
echo DirectX SDK not found in default locations.
echo.
echo Options:
echo 1) Download DirectX SDK (approx. 570MB)
echo 2) Specify existing DirectX SDK path
echo.

choice /c 12 /m "Choose an option"
if errorlevel 2 goto manual_path
if errorlevel 1 goto download_sdk

:download_sdk
echo.
echo Downloading DirectX SDK (June 2010) from Microsoft...
echo URL: %DIRECTX_URL%
echo This will take some time (approx. 570MB)...

if not exist "%DIRECTX_EXE%" (
    echo Downloading with PowerShell...
    powershell -Command "$progressPreference = 'silentlyContinue'; Invoke-WebRequest -Uri '%DIRECTX_URL%' -OutFile '%DIRECTX_EXE%'"
    
    if not exist "%DIRECTX_EXE%" (
        echo ERROR: Download failed. Please download manually from:
        echo https://www.microsoft.com/en-us/download/details.aspx?id=6812
        goto manual_path
    )
    
    echo Download completed!
) else (
    echo Found previously downloaded installer.
)

echo.
echo Installing DirectX SDK (June 2010)...
echo IMPORTANT: 
echo - Follow the installation prompts
echo - If you get S1023 error, uninstall VC++ 2010 Redistributables first
echo   (Control Panel -^> Programs and Features -^> Uninstall Visual C++ 2010)
echo.

choice /c YN /m "Launch installer now"
if errorlevel 2 goto manual_path
if errorlevel 1 (
    start "" "%DIRECTX_EXE%"
    echo.
    echo After installation completes, press any key to continue...
    pause >nul
    
    REM Check if environment variable was set by installer
    set "DXSDK_DIR="
    if defined DXSDK_DIR (
        if exist "%DXSDK_DIR%" (
            echo Found DirectX SDK environment variable: %DXSDK_DIR%
            set "DXSDK_PATH=%DXSDK_DIR%"
            goto copy_directx_files
        )
    )
    
    REM Check common locations after install
    if exist "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)" (
        set "DXSDK_PATH=C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)"
        goto copy_directx_files
    ) else if exist "C:\Program Files\Microsoft DirectX SDK (June 2010)" (
        set "DXSDK_PATH=C:\Program Files\Microsoft DirectX SDK (June 2010)"
        goto copy_directx_files
    )
    
    echo Could not find newly installed DirectX SDK.
    goto manual_path
)

:manual_path
echo.
echo Please enter the path to your DirectX SDK installation.
echo Default locations are:
echo  - C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)
echo  - C:\Program Files\Microsoft DirectX SDK (June 2010)
echo.

set "DXSDK_PATH="
set /p DXSDK_PATH="Enter path: "

if not exist "%DXSDK_PATH%" (
    echo.
    echo ERROR: Path not found: %DXSDK_PATH%
    
    choice /c YN /m "Try again"
    if errorlevel 2 goto end
    if errorlevel 1 goto manual_path
)

:copy_directx_files
echo.
echo Copying DirectX files from: %DXSDK_PATH%
echo.

REM Copy DirectX 8 files
echo Copying DirectX 8 headers...
for %%f in (d3d8.h d3d8caps.h d3d8types.h d3dx8.h d3dx8core.h d3dx8math.h d3dx8mesh.h d3dx8tex.h) do (
    echo   Copying %%f
    copy /y "%DXSDK_PATH%\Include\%%f" "%DIRECTX_DIR%\Include\" >nul 2>&1
    if errorlevel 1 echo   - Failed to copy %%f
)

echo Copying DirectX 8 libraries...
for %%f in (d3d8.lib d3dx8.lib d3dx8d.lib) do (
    echo   Copying %%f
    copy /y "%DXSDK_PATH%\Lib\x86\%%f" "%DIRECTX_DIR%\Lib\" >nul 2>&1
    if errorlevel 1 echo   - Failed to copy %%f
)

REM Copy DirectX 9 files (if missing)
echo Copying DirectX 9 headers...
for %%f in (d3d9.h d3d9caps.h d3d9types.h d3dx9.h d3dx9core.h d3dx9math.h d3dx9mesh.h d3dx9tex.h dinput.h dsound.h dxerr9.h dxfile.h dxguid.h) do (
    echo   Copying %%f
    copy /y "%DXSDK_PATH%\Include\%%f" "%DIRECTX_DIR%\Include\" >nul 2>&1
    if errorlevel 1 echo   - Failed to copy %%f
)

echo Copying DirectX 9 libraries...
for %%f in (d3d9.lib d3dx9.lib d3dx9d.lib dinput8.lib dsound.lib dxguid.lib) do (
    echo   Copying %%f
    copy /y "%DXSDK_PATH%\Lib\x86\%%f" "%DIRECTX_DIR%\Lib\" >nul 2>&1
    if errorlevel 1 echo   - Failed to copy %%f
)

echo.
echo DirectX SDK setup completed!
goto finished

:finished
echo.
echo ====================================================================
echo DirectX setup process completed!
echo ====================================================================
echo.

REM List the contents of the DirectX folders to verify
echo DirectX Include Files:
dir "%DIRECTX_DIR%\Include\"

echo.
echo DirectX Library Files:
dir "%DIRECTX_DIR%\Lib\"

:end
echo.
pause 
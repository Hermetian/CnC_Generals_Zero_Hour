@echo off
echo.
echo ====================================================================
echo SETTING UP STLPORT-4.5.3 LIBRARY
echo ====================================================================
echo.

REM Set paths
set REPO_ROOT=%~dp0..
set GENERALSMD_DIR=%REPO_ROOT%\GeneralsMD
set CODE_DIR=%GENERALSMD_DIR%\Code
set STLPORT_DIR=%CODE_DIR%\Libraries\Source\STLport-4.5.3
set TEMP_DIR=%REPO_ROOT%\temp_downloads
set STLPORT_ZIP=%TEMP_DIR%\stlport-4.5.3.zip
set STLPORT_URL=https://sourceforge.net/projects/stlport/files/STLport/STLport%204.5.3/STLport-4.5.3.zip/download

REM Create temp directory if it doesn't exist
if not exist "%TEMP_DIR%" mkdir "%TEMP_DIR%"

echo Step 1: Downloading STLport-4.5.3
echo ----------------------------------------
echo.

echo Downloading STLport-4.5.3 from SourceForge...
echo URL: %STLPORT_URL%
echo.

powershell -Command "& {[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri '%STLPORT_URL%' -OutFile '%STLPORT_ZIP%'}"

if not exist "%STLPORT_ZIP%" (
    echo ERROR: Failed to download STLport-4.5.3.
    goto :error
)

echo Download completed successfully.
echo.

echo Step 2: Extracting STLport-4.5.3
echo ----------------------------------------
echo.

echo Extracting STLport-4.5.3 to %STLPORT_DIR%...
echo.

REM Create STLport directory if it doesn't exist
if not exist "%STLPORT_DIR%" mkdir "%STLPORT_DIR%"

powershell -Command "& {Add-Type -AssemblyName System.IO.Compression.FileSystem; [System.IO.Compression.ZipFile]::ExtractToDirectory('%STLPORT_ZIP%', '%STLPORT_DIR%')}"

echo Extraction completed.
echo.

echo Step 3: Applying STLport patch
echo ----------------------------------------
echo.

echo Applying patch from %REPO_ROOT%\stlport.diff...
echo.

REM Check if patch command is available
where patch >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo WARNING: 'patch' command not found.
    echo You will need to manually apply the patch from %REPO_ROOT%\stlport.diff
    echo to the STLport-4.5.3 files.
    echo.
    echo Please see the README.md for more information.
) else (
    cd "%STLPORT_DIR%"
    patch -p0 < "%REPO_ROOT%\stlport.diff"
    cd "%REPO_ROOT%\Support"
    echo Patch applied successfully.
)

echo.
echo STLport-4.5.3 setup completed!
echo.
echo Next steps:
echo 1. Run build_all.bat to build the project
echo 2. Check for any compilation errors related to STLport
echo.
goto :end

:error
echo.
echo STLport-4.5.3 setup failed.
echo.

:end
pause 
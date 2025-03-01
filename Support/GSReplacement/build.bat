@echo off
echo "Building GameSpy Replacement for Command & Conquer: Generals - Zero Hour"
echo.

:: Check if CMake is installed
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Error: CMake is not installed or not in the PATH.
    echo Please install CMake from https://cmake.org/download/
    exit /b 1
)

:: Try to find Visual Studio using vswhere tool (included with Visual Studio 2017 and later)
echo Looking for Visual Studio installation...
set VSWHERE="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist %VSWHERE% (
    for /f "usebackq delims=" %%i in (`%VSWHERE% -latest -prerelease -property installationPath`) do (
        if exist "%%i\Common7\Tools\VsDevCmd.bat" (
            echo Found Visual Studio at: %%i
            call "%%i\Common7\Tools\VsDevCmd.bat" -no_logo
            goto vs_found
        )
    )
)

:: Manual fallback to common Visual Studio locations
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -no_logo
    goto vs_found
)

if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat" -no_logo
    goto vs_found
)

echo Error: Could not find Visual Studio installation.
echo Please install Visual Studio from https://visualstudio.microsoft.com/
exit /b 1

:vs_found
echo Visual Studio environment set up successfully.

:: Create build directory
if not exist build mkdir build
cd build

:: Configure with CMake
echo Configuring with CMake...
cmake -Wdev --debug-output ..

:: Build the project
echo Building project...
cmake --build . --config Release

:: Check if build succeeded
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Build failed. See error messages above.
    exit /b 1
)

echo.
echo Build completed successfully!
echo.
echo Output files are in the build\Release directory:
echo - gamespy.dll: DLL to replace the original GameSpy DLL
echo - GSReplacementApp.exe: Standalone application for testing

cd ..
pause 
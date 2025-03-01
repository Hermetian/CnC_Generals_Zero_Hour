@echo off
REM LZH-Light 1.0 Setup Script for C&C Generals Zero Hour
REM This script downloads LZH-Light source files from GitHub and places them in the correct directories

setlocal enabledelayedexpansion

REM Set paths - Fix for running from Support directory
set "REPO_ROOT=%CD%\.."
set "TMP_DIR=%REPO_ROOT%\Support\tmp"
set "SOURCE_DIR=%REPO_ROOT%\GeneralsMD\Code\Libraries\Source\Compression\LZHCompress\CompLibSource"
set "HEADER_DIR=%REPO_ROOT%\GeneralsMD\Code\Libraries\Source\Compression\LZHCompress\CompLibHeader"
set "LZHL_REPO=https://github.com/ryandrake08/lzhl/archive/refs/heads/master.zip"
set "LZHL_ZIP=%TMP_DIR%\lzhl-master.zip"
set "EXTRACT_PATH=%TMP_DIR%\lzhl-extract"
set "LZHL_DIR=%EXTRACT_PATH%\lzhl-master"
set "TABLE_DIR=%LZHL_DIR%\Table"

echo Repository root: %REPO_ROOT%
echo Temporary directory: %TMP_DIR%
echo Source files directory: %SOURCE_DIR%
echo Header files directory: %HEADER_DIR%

REM Create temporary directory if it doesn't exist
if not exist "%TMP_DIR%" (
    mkdir "%TMP_DIR%"
    echo Created temporary directory: %TMP_DIR%
)

REM Create source and header directories if they don't exist
if not exist "%SOURCE_DIR%" (
    mkdir "%SOURCE_DIR%"
    echo Created source directory: %SOURCE_DIR%
)

if not exist "%HEADER_DIR%" (
    mkdir "%HEADER_DIR%"
    echo Created header directory: %HEADER_DIR%
)

REM Download LZH-Light from GitHub
echo Downloading LZH-Light from %LZHL_REPO%...
powershell -Command "& {[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri '%LZHL_REPO%' -OutFile '%LZHL_ZIP%'}"
if %ERRORLEVEL% NEQ 0 (
    echo Error downloading LZH-Light.
    exit /b 1
) else (
    echo Download completed successfully: %LZHL_ZIP%
)

REM Extract LZH-Light
echo Extracting LZH-Light...
if exist "%EXTRACT_PATH%" rmdir /s /q "%EXTRACT_PATH%"
mkdir "%EXTRACT_PATH%"

powershell -Command "& {Add-Type -AssemblyName System.IO.Compression.FileSystem; [System.IO.Compression.ZipFile]::ExtractToDirectory('%LZHL_ZIP%', '%EXTRACT_PATH%')}"
if %ERRORLEVEL% NEQ 0 (
    echo Error extracting LZH-Light.
    exit /b 1
) else (
    echo Extraction completed successfully to: %EXTRACT_PATH%
)

REM Copy source files
echo Copying source files...
set COPY_ERRORS=0

REM Create Huffman table files manually
echo Creating Huffman table files manually...

REM Create empty table files
echo // LZH-Light Huffman table file > "%SOURCE_DIR%\Hdec_g.tbl"
echo // LZH-Light Huffman table file > "%SOURCE_DIR%\Hdec_s.tbl"
echo // LZH-Light Huffman table file > "%SOURCE_DIR%\Hdisp.tbl"
echo // LZH-Light Huffman table file > "%SOURCE_DIR%\Henc.tbl"
echo Created placeholder table files

REM Source code files
if exist "%LZHL_DIR%\HuffStat.cpp" (
    copy /Y "%LZHL_DIR%\HuffStat.cpp" "%SOURCE_DIR%\Huff.cpp" >nul
    echo Copied Huff.cpp
) else (
    echo Warning: Could not find source file: HuffStat.cpp
    set /a COPY_ERRORS+=1
)

if exist "%LZHL_DIR%\LZBuffer.cpp" (
    copy /Y "%LZHL_DIR%\LZBuffer.cpp" "%SOURCE_DIR%\Lz.cpp" >nul
    echo Copied Lz.cpp
) else (
    echo Warning: Could not find source file: LZBuffer.cpp
    set /a COPY_ERRORS+=1
)

if exist "%LZHL_DIR%\LZHL.cpp" (
    copy /Y "%LZHL_DIR%\LZHL.cpp" "%SOURCE_DIR%\Lzhl.cpp" >nul
    echo Copied Lzhl.cpp
) else (
    echo Warning: Could not find source file: LZHL.cpp
    set /a COPY_ERRORS+=1
)

if exist "%LZHL_DIR%\LZHLDecoderStat.cpp" (
    copy /Y "%LZHL_DIR%\LZHLDecoderStat.cpp" "%SOURCE_DIR%\Lzhl_tcp.cpp" >nul
    echo Copied Lzhl_tcp.cpp
) else (
    echo Warning: Could not find source file: LZHLDecoderStat.cpp
    set /a COPY_ERRORS+=1
)

REM Copy header files
echo Copying header files...

if exist "%LZHL_DIR%\HuffStat.hpp" (
    copy /Y "%LZHL_DIR%\HuffStat.hpp" "%HEADER_DIR%\_huff.h" >nul
    echo Copied _huff.h
) else (
    echo Warning: Could not find source file: HuffStat.hpp
    set /a COPY_ERRORS+=1
)

if exist "%LZHL_DIR%\LZBuffer.hpp" (
    copy /Y "%LZHL_DIR%\LZBuffer.hpp" "%HEADER_DIR%\_lz.h" >nul
    echo Copied _lz.h
) else (
    echo Warning: Could not find source file: LZBuffer.hpp
    set /a COPY_ERRORS+=1
)

if exist "%LZHL_DIR%\LZHMacro.hpp" (
    copy /Y "%LZHL_DIR%\LZHMacro.hpp" "%HEADER_DIR%\_lzhl.h" >nul
    echo Copied _lzhl.h
) else (
    echo Warning: Could not find source file: LZHMacro.hpp
    set /a COPY_ERRORS+=1
)

if exist "%LZHL_DIR%\LZHL.h" (
    copy /Y "%LZHL_DIR%\LZHL.h" "%HEADER_DIR%\Lzhl.h" >nul
    echo Copied Lzhl.h
) else (
    echo Warning: Could not find source file: LZHL.h
    set /a COPY_ERRORS+=1
)

if exist "%LZHL_DIR%\LZHLDecoderStat.hpp" (
    copy /Y "%LZHL_DIR%\LZHLDecoderStat.hpp" "%HEADER_DIR%\Lzhl_tcp.h" >nul
    echo Copied Lzhl_tcp.h
) else (
    echo Warning: Could not find source file: LZHLDecoderStat.hpp
    set /a COPY_ERRORS+=1
)

REM Cleanup
echo Cleaning up temporary files...
if exist "%LZHL_ZIP%" del /F "%LZHL_ZIP%"
if exist "%EXTRACT_PATH%" rmdir /s /q "%EXTRACT_PATH%"

REM Final status report
if %COPY_ERRORS% EQU 0 (
    echo LZH-Light 1.0 has been successfully set up!
    
    echo.
    echo Files in the source directory:
    for %%F in ("%SOURCE_DIR%\*.*") do (
        echo - %%~nxF
    )
    
    echo.
    echo Files in the header directory:
    for %%F in ("%HEADER_DIR%\*.*") do (
        echo - %%~nxF
    )
) else (
    echo LZH-Light setup completed with %COPY_ERRORS% errors. Please check the output above.
)

echo.
echo Note: The Huffman table files (*.tbl) are placeholders. The modern LZHL code generates
echo       these tables dynamically rather than using static files.
echo.
echo Press any key to continue...
pause > nul 
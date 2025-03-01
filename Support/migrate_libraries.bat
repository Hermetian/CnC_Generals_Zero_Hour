@echo off
echo.
echo ====================================================================
echo MIGRATING LIBRARIES TO ZERO HOUR (GENERALSMD) STRUCTURE
echo ====================================================================
echo.

REM Create migration log
set LOG_FILE=migration_log.txt
echo Migration started at %date% %time% > %LOG_FILE%

REM Define source and destination paths
set SOURCE_DIR=Code\Libraries
set DEST_DIR=CnC_Generals_Zero_Hour\GeneralsMD\Code\Libraries

REM Create destination directories if they don't exist
if not exist "%DEST_DIR%" mkdir "%DEST_DIR%"
if not exist "%DEST_DIR%\Max4SDK" mkdir "%DEST_DIR%\Max4SDK"
if not exist "%DEST_DIR%\DirectX" mkdir "%DEST_DIR%\DirectX"
if not exist "%DEST_DIR%\STLport-4.5.3" mkdir "%DEST_DIR%\STLport-4.5.3"

echo.
echo Step 1: Moving Max4SDK (Stub Implementation)
echo ----------------------------------------
echo.

REM Move Max4SDK files
echo Moving Max4SDK files... >> %LOG_FILE%
if exist "%SOURCE_DIR%\Max4SDK" (
    echo Moving Max4SDK stub implementation...
    xcopy /E /I /Y "%SOURCE_DIR%\Max4SDK\*" "%DEST_DIR%\Max4SDK\"
    echo Max4SDK files moved successfully! >> %LOG_FILE%
) else (
    echo WARNING: Max4SDK directory not found in source.
    echo WARNING: Max4SDK directory not found in source. >> %LOG_FILE%
)

echo.
echo Step 2: Moving DirectX Libraries
echo ----------------------------------------
echo.

REM Move DirectX files
echo Moving DirectX files... >> %LOG_FILE%
if exist "%SOURCE_DIR%\DirectX" (
    echo Moving DirectX libraries...
    xcopy /E /I /Y "%SOURCE_DIR%\DirectX\*" "%DEST_DIR%\DirectX\"
    echo DirectX files moved successfully! >> %LOG_FILE%
) else (
    echo WARNING: DirectX directory not found in source.
    echo WARNING: DirectX directory not found in source. >> %LOG_FILE%
)

echo.
echo Step 3: Moving STLport Libraries
echo ----------------------------------------
echo.

REM Move STLport files
echo Moving STLport files... >> %LOG_FILE%
if exist "%SOURCE_DIR%\STLport-4.5.3" (
    echo Moving STLport libraries...
    xcopy /E /I /Y "%SOURCE_DIR%\STLport-4.5.3\*" "%DEST_DIR%\STLport-4.5.3\"
    echo STLport files moved successfully! >> %LOG_FILE%
) else (
    echo WARNING: STLport directory not found in source.
    echo WARNING: STLport directory not found in source. >> %LOG_FILE%
)

echo.
echo Step 4: Moving ZLib and LZH from Source directory
echo ----------------------------------------
echo.

REM Move ZLib and LZH files
echo Moving compression libraries... >> %LOG_FILE%
if not exist "%DEST_DIR%\Source\Compression" mkdir "%DEST_DIR%\Source\Compression"

if exist "%SOURCE_DIR%\Source\Compression\ZLib" (
    echo Moving ZLib libraries...
    xcopy /E /I /Y "%SOURCE_DIR%\Source\Compression\ZLib\*" "%DEST_DIR%\Source\Compression\ZLib\"
    echo ZLib files moved successfully! >> %LOG_FILE%
) else (
    echo WARNING: ZLib directory not found in source.
    echo WARNING: ZLib directory not found in source. >> %LOG_FILE%
)

if exist "%SOURCE_DIR%\Source\Compression\LZHCompress" (
    echo Moving LZH libraries...
    xcopy /E /I /Y "%SOURCE_DIR%\Source\Compression\LZHCompress\*" "%DEST_DIR%\Source\Compression\LZHCompress\"
    echo LZH files moved successfully! >> %LOG_FILE%
) else (
    echo WARNING: LZHCompress directory not found in source.
    echo WARNING: LZHCompress directory not found in source. >> %LOG_FILE%
)

echo.
echo Step 5: Moving WPAudio directory
echo ----------------------------------------
echo.

REM Move WPAudio files
echo Moving WPAudio files... >> %LOG_FILE%
if exist "%SOURCE_DIR%\WPAudio" (
    echo Moving WPAudio libraries...
    if not exist "%DEST_DIR%\WPAudio" mkdir "%DEST_DIR%\WPAudio"
    xcopy /E /I /Y "%SOURCE_DIR%\WPAudio\*" "%DEST_DIR%\WPAudio\"
    echo WPAudio files moved successfully! >> %LOG_FILE%
) else (
    echo WARNING: WPAudio directory not found in source.
    echo WARNING: WPAudio directory not found in source. >> %LOG_FILE%
)

echo.
echo Migration complete!
echo ====================================================================
echo.
echo All libraries have been migrated to the Zero Hour (GeneralsMD) structure.
echo A migration log has been created at: %LOG_FILE%
echo.
echo Next steps:
echo 1. Update your build configuration to point to the new locations
echo 2. Test the build process with the new structure
echo 3. After confirming everything works, you can remove the old Code directory
echo.

REM Update the migration log
echo Migration completed at %date% %time% >> %LOG_FILE%
echo. >> %LOG_FILE%
echo Next steps: >> %LOG_FILE%
echo 1. Update build configuration to point to the new locations >> %LOG_FILE%
echo 2. Test the build process with the new structure >> %LOG_FILE%
echo 3. After confirming everything works, remove the old Code directory >> %LOG_FILE%

pause 
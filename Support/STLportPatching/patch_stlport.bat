@echo off
REM Patch script for STLport-4.5.3 for Command and Conquer Generals
REM This script will apply stlport.diff to the STLport library

SETLOCAL ENABLEDELAYEDEXPANSION

REM Check if python is available using 'py' command
py --version > nul 2>&1
IF %ERRORLEVEL% EQU 0 (
    SET PYTHON_CMD=py
) ELSE (
    REM Try 'python' command as fallback
    python --version > nul 2>&1
    IF %ERRORLEVEL% EQU 0 (
        SET PYTHON_CMD=python
    ) ELSE (
        ECHO Python not found. Please install Python to run this script.
        EXIT /B 1
    )
)

REM Parse command-line arguments
SET DRY_RUN=
SET CUSTOM_PATH=
SET PROJECT_ROOT=..\..

:PARSE_ARGS
IF "%~1"=="" GOTO END_PARSE_ARGS
IF "%~1"=="--dry-run" (
    SET DRY_RUN=--dry-run
) ELSE IF "%~1"=="--root" (
    SHIFT
    SET PROJECT_ROOT=%~1
) ELSE (
    SET CUSTOM_PATH=%~1
)
SHIFT
GOTO PARSE_ARGS
:END_PARSE_ARGS

ECHO Running STLport patching script...
ECHO -----------------------------------

REM If custom path is provided, use it directly
IF NOT "%CUSTOM_PATH%"=="" (
    IF EXIST "%CUSTOM_PATH%" (
        ECHO Using custom STLport path: %CUSTOM_PATH%
        %PYTHON_CMD% apply_stlport_diff.py --stlport-dir "%CUSTOM_PATH%" %DRY_RUN%
    ) ELSE (
        ECHO Custom STLport path not found: %CUSTOM_PATH%
        EXIT /B 1
    )
) ELSE (
    REM Otherwise, let the Python script find STLport directories automatically
    ECHO Searching for STLport directories automatically...
    %PYTHON_CMD% apply_stlport_diff.py --project-root "%PROJECT_ROOT%" %DRY_RUN%
)

IF %ERRORLEVEL% NEQ 0 (
    ECHO Patch application failed with error code %ERRORLEVEL%
    EXIT /B %ERRORLEVEL%
)

ECHO Patch application completed.
EXIT /B 0 
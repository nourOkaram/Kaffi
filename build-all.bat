@ECHO OFF
REM Master build script for the entire project on Windows.
REM This script orchestrates the build process by calling the individual
REM build scripts for each component in the correct order.
REM This file MUST be located inside the workspace directory.

REM Define WORKSPACE if not already defined
IF NOT DEFINED WORKSPACE (
    SET "WORKSPACE=%~dp0"
    REM Remove trailing backslash if present using CALL
    CALL (IF "%%WORKSPACE:~-1%%"=="\" || "%%WORKSPACE:~-1%%"=="/") (
        CALL SET "WORKSPACE=%%WORKSPACE:~0,-1%%"
    )
)

IF NOT DEFINED BIN_DIR SET "BIN_DIR=%WORKSPACE%\bin"

CD /D "%WORKSPACE%"


ECHO Building everything...

REM Define a space-separated list of components (assemblies) to build.
REM The order is critical: 'engine' must be built before 'testbed' because
REM testbed depends on the library file (engine.lib) created by the engine build.
SET assemblies=engine testbed

REM Enable delayed expansion to correctly read the ERRORLEVEL variable inside the loop.
SETLOCAL ENABLEDELAYEDEXPANSION

REM Loop through each assembly defined in the 'assemblies' variable.
FOR %%F IN (%assemblies%) DO (

    REM Temporarily change the current directory to the assembly's folder (e.g., 'engine').
    PUSHD %%F

    REM Execute the build script located in that directory.
    CALL build.bat

    REM Return to the previous directory.
    POPD

    REM Check the exit code of the last command (build.bat).
    REM A non-zero ERRORLEVEL indicates that the build failed.
    IF !ERRORLEVEL! NEQ 0 (
        ECHO [ERROR]: Build failed in '%%F' with error code: !ERRORLEVEL!
        ECHO Halting the build process.

        REM Exit the entire script, passing the error code up.
        EXIT /B !ERRORLEVEL!
    )
)

REM If the loop completes without any errors, all components were built successfully.
ECHO ALL assemblies built successfully.
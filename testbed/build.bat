@ECHO OFF
REM Build script for the Testbed application on Windows.
REM This script performs the following steps:
REM 1. Finds all .c source files for the testbed.
REM 2. Compiles the source files.
REM 3. Links the compiled objects with the engine.lib into a single executable (testbed.exe).

REM Enable delayed expansion to properly handle the C file list within the loop
SetLocal EnableDelayedExpansion

REM --- Check required environment variables using FOR loop ---

REM Defines an array of required variable names.
SET REQUIRED_VARS=WORKSPACE BIN_DIR VULKAN_SDK

REM Loop through the array.
FOR %%V IN (%REQUIRED_VARS%) DO (
    REM Check if the variable is unset or empty. ${!VAR_NAME} is used for indirect variable expansion.
    IF "!%%V!"=="" (
        ECHO [ERROR]: %%V not defined. Please set %%V environment variable before building!
        EXIT /B 1
    )
)

IF NOT EXIST "%BIN_DIR%" (
    mkdir "%BIN_DIR%"
    echo Folder "%BIN_DIR%" created.
)

REM --- Step 1: Collect all C source files ---
REM Recursively search the current directory and create a space-separated list of all .c files.
SET cFilenames=
FOR /R %%f in (*.c) do (
    SET cFilenames=!cFilenames! %%f
)

REM echo "Files:" %cFilenames%

REM --- Step 2: Define build variables ---

REM Define the base name for the output executable (e.g., testbed.exe).
SET assembly=testbed

REM Flags for the compiler.
SET compilerFlags=-g
REM -g                      : Include debug information.

REM Specify paths for the compiler to locate header files.
SET includeFlags=-Isrc -I%WORKSPACE%/engine/src
REM -Isrc                   : Search for headers in the local 'src' directory (if any).
REM -I../engine/src         : Search for the engine's public headers (like test.h).

REM Libraries and their directories for the linker.
SET linkerFlags=-L%BIN_DIR% -lengine
REM -lengine                : Link against the 'engine' library. Clang will look for 'engine.lib'
REM                           in the search paths.
REM -L"%BIN_DIR%"           : Add the binary output directory (where engine.lib is located)
REM                           to the linker's search paths.

REM Preprocessor definitions to pass to the compiler.
SET defines=-D_DEBUG -DKIMPORT
REM -D_DEBUG                    : Define the _DEBUG macro, usually for debug-only code.
REM -DKIMPORT                   : Define the KIMPORT macro, usually for import-related functionality.



REM Define the full output path and filename for the executable.
SET OUTPUT_EXE=%BIN_DIR%/%assembly%.exe

REM --- Step 3: Compile all source files and link into the executable ---
ECHO Building %assembly%...

clang %cFilenames% %compilerFlags% -o %OUTPUT_EXE% %defines% %includeFlags% %linkerFlags%
REM Invoke clang with the collected source files and compiler/linker flags.

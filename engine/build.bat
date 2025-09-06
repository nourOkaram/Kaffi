@ECHO OFF
REM Build script for the Engine library on Windows
REM This script performs the following steps:
REM 1. Finds all .c source files in the project
REM 2. Compiles each source file
REM 3. Links the compiled objects into a single dynamic-link library (engine.dll)

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
REM Recursively search the current directory and create a space-separated list of all .c files
SET cFilenames=
FOR /R %%f in (*.c) do (
    SET cFilenames=!cFilenames! %%f
)

REM echo "Files:" %cFilenames%

REM --- Step 2: Define build variables ---

REM Define the base name for the output file (e.g., engine.dll)
SET assembly=engine

REM Flags for the compiler.
SET compilerFlags=-g -shared -Wvarargs -Wall -Werror
REM -g                      : Include debug information
REM -shared                 : Instruct clang to create a shared library (.dll)
REM -Wvarargs -Wall -Werror : Enable extensive warnings and treat them as errors

REM Specify paths for the compiler to locate header files
SET includeFlags=-Isrc -I%VULKAN_SDK%/Include
REM -Isrc                   : Search for headers in the local 'src' directory
REM -I%VULKAN_SDK%/Include  : Search for headers in the Vulkan SDK's 'Include' directory

REM Libraries and their directories for the linker
SET linkerFlags=-L%VULKAN_SDK%\Lib -lvulkan-1 -luser32
REM -luser32             : Link against the Windows User32 library (for windowing and related functionality)
REM -lvulkan-1           : Link against the Vulkan library
REM -L%VULKAN_SDK%/Lib   : Add the Vulkan SDK 'Lib' directory to the linker's search paths

REM Preprocessor definitions to pass to the compiler
SET defines=-D_DEBUG -DKEXPORT -D_CRT_SECURE_NO_WARNINGS
REM -D_DEBUG                    : Define the _DEBUG macro, usually for debug-only code
REM -DKEXPORT                   : CRITICAL! Defines KEXPORT, signaling defines.h to apply 'dllexport'
REM                                 to KAPI functions, making them visible outside the DLL
REM -D_CRT_SECURE_NO_WARNINGS   : Suppress common security warnings from the Microsoft C Runtime


REM Define the full output path and filename for the shared object.
SET OUTPUT_DLL=%BIN_DIR%/%assembly%.dll

REM --- Step 3: Compile all source files and link into the DLL ---
ECHO Building %assembly%...

clang %cFilenames% %compilerFlags% -o %OUTPUT_DLL% %defines% %includeFlags% %linkerFlags% >nul 2>&1
REM Invoke clang with the collected source files and compiler/linker flags


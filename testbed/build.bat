REM Build script for testbed
@ECHO OFF

SetLocal EnableDelayedExpansion

REM --- Check required environment variables using FOR loop ---
FOR %%V IN (WORKSPACE BIN_DIR VULKAN_SDK) DO (
    IF "!%%V!"=="" (
        ECHO [ERROR]: %%V not defined. Please set %%V environment variable before building!
        EXIT /B 1
    )
)

IF NOT EXIST "%BIN_DIR%" (
    mkdir "%BIN_DIR%"
    echo Folder "%BIN_DIR%" created.
)

REM Get a list of all the .c files
SET cFilenames=

FOR /R %%f in (*.c) do (
    SET cFilenames=!cFilenames! %%f
)

REM echo "Files:" %cFilenames%

SET assembly=testbed
SET compilerFlags=-g
SET includeFlags=-Isrc -I%WORKSPACE%/engine/src
SET linkerFlags=-L%BIN_DIR% -lengine
SET defines=-D_DEBUG -DKIMPORT

REM Output file
SET OUTPUT_EXE=%BIN_DIR%/%assembly%.exe

ECHO Building %assembly%...
clang %cFilenames% %compilerFlags% -o %OUTPUT_EXE% %defines% %includeFlags% %linkerFlags%

REM Build script for engine
@ECHO OFF

SetLocal EnableDelayedExpansion

REM --- Check required environment variables using FOR loop ---
FOR %%V IN (WORKSPACE BIN_DIR) DO (
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

SET assembly=engine
SET compilerFlags=-g -shared -Wvarargs -Wall -Werror
SET includeFlags=-Isrc -I%VULKAN_SDK%/Include
SET linkerFlags=-L%VULKAN_SDK%\Lib -lvulkan-1 -luser32
SET defines=-D_DEBUG -DKEXPORT -D_CRT_SECURE_NO_WARNINGS

REM Output file
SET OUTPUT_DLL=%BIN_DIR%/%assembly%.dll

ECHO Building %assembly%...
clang %cFilenames% %compilerFlags% -o %OUTPUT_DLL% %defines% %includeFlags% %linkerFlags%

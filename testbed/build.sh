#!/bin/bash
# Build script for the Testbed application on Linux.
# This script performs the following steps:
# 1. Finds all .c source files for the testbed.
# 2. Compiles the source files.
# 3. Links the compiled objects with the engine's shared object (libengine.so)
#    to create a single executable (testbed).

# Exit immediately if a command exits with a non-zero status.
set -e

# --- Check required environment variables ---
# Defines an array of required variable names.
REQUIRED_VARS=("WORKSPACE" "BIN_DIR")

# Loop through the array.
for VAR_NAME in "${REQUIRED_VARS[@]}"; do
    # Check if the variable is unset or empty.
    if [ -z "${!VAR_NAME}" ]; then
        echo "[ERROR]: ${VAR_NAME} not defined. Please set the ${VAR_NAME} environment variable before building!"
        exit 1
    fi
done

# Ensure the binary output directory exists.
# '-p' creates parent directories as needed and ignores existing ones.
if [ ! -d "$BIN_DIR" ]; then
    mkdir -p "$BIN_DIR"
    echo "Folder '$BIN_DIR' created."
fi

# --- Step 1: Collect all C source files ---
# Use 'find' to create a space-separated list of all files ending with .c.
cFilenames=$(find . -type f -name "*.c")

# echo "Files:$cFilenames"


# --- Step 2: Define build variables ---

# Define the base name for the output executable (e.g., testbed).
assembly="testbed"

# Flags for the compiler.
compilerFlags="-g -fdeclspec -fPIC"
# -g                        : Include debug information.
# -fdeclspec                : (Feature) Support __declspec (Windows compatibility).
# -fPIC                     : (Feature) Generate Position-Independent Code

# Specify paths for the compiler to locate header files.
includeFlags="-Isrc -I$WORKSPACE/engine/src"
# -Isrc                           : Search for headers in the local 'src' directory (if any).
# -I$WORKSPACE/engine/src         : Search for the engine's public headers (like test.h).

# Libraries and their directories for the linker.
linkerFlags="-L$BIN_DIR -lengine -Wl,-rpath,."
# -lengine                : Link against the 'engine' library (looks for libengine.so).
# -L"${BIN_DIR}"          : Add the binary output directory (where libengine.so is located)
#                           to the linker's search paths.
# -Wl,-rpath,.            : CRITICAL! Embeds a runtime search path in the executable. '.'
#                           is a special path that tells the dynamic linker to look for shared
#                           libraries in the same directory as the executable itself.

# Preprocessor definitions to pass to the compiler.
defines="-D_DEBUG -DKIXPORT"
# -D_DEBUG                : Define the _DEBUG macro.
# -DKIMPORT               : Define the KIMPORT macro, usually for import-related functionality.

# Define the full output path and filename for the executable.
OUTPUT_EX="$BIN_DIR/$assembly" 

# --- Step 3: Compile all source files and link into the executable ---
echo "Building $assembly..."

clang $cFilenames $compilerFlags -o $OUTPUT_EX $defines $includeFlags $linkerFlags
# Invoke clang with the collected source files and compiler/linker flags.

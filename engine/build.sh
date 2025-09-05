#!/bin/bash
# Build script for the Engine library on Linux.
# This script performs the following steps:
# 1. Finds all .c source files in the project.
# 2. Compiles each source file into a position-independent object.
# 3. Links the compiled objects into a single shared object (libengine.so).

# Exit immediately if a command exits with a non-zero status.
set -e

# --- Check required environment variables ---

# Defines an array of required variable names.
REQUIRED_VARS=("WORKSPACE" "BIN_DIR" "VULKAN_SDK")

# Loop through the array.
for VAR_NAME in ${REQUIRED_VARS[@]}; do
    # Check if the variable is unset or empty. ${!VAR_NAME} is used for indirect variable expansion.
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

# Define the base name for the output library (e.g., libengine.so).
assembly="engine"

# Flags for the compiler.
compilerFlags="-g -shared -fdeclspec -fPIC"
# -g                        : Include debug information.
# -shared                   : Instruct clang to create a shared object (.so).
# -fdeclspec                : (Feature) Support __declspec (Windows compatibility).
# -fPIC                     : (Feature) Generate Position-Independent Code

# Specify paths for the compiler to locate header files.
includeFlags="-Isrc -I$VULKAN_SDK/include"
# -Isrc                     : Search for headers in the local 'src' directory.
# -I$VULKAN_SDK/include     : Search for headers in the Vulkan SDK's 'include' directory.

# Libraries and their directories for the linker.
linkerFlags="-L$VULKAN_SDK/lib -L/usr/lib/x86_64-linux-gnu -lvulkan -lxcb -lX11 -lX11-xcb -lxkbcommon"
# -lvulkan                      : Link against the Vulkan library.
# -lxcb                         : Link against the XCB library (X protocol C-language Binding).
# -lX11                         : Link against the main X11 library (Xlib, high-level interface to X11 protocol)
# -lX11-xcb                     : Link against the X11-XCB integration library.
# -lxkbcommon-x11               : Link against the XKB common library for advanced keyboard handling.
# -L$VULKAN_SDK/lib             : Add the Vulkan SDK 'lib' directory to the linker's search paths.
# -L/usr/lib/x86_64-linux-gnu   : Add standard 64-bit Linux lib directory to linker's search path.

# Preprocessor definitions to pass to the compiler.
defines="-D_DEBUG -DKEXPORT"
# -D_DEBUG              : Define the _DEBUG macro, usually for debug-only code.
# -DKEXPORT             : CRITICAL! Defines KEXPORT, signaling defines.h to set the correct
#                         visibility attributes for functions, making them visible outside the .so file

# Define the full output path and filename for the shared object.
OUTPUT_DLL="$BIN_DIR/lib$assembly.so"

# --- Step 3: Compile all source files and link into the shared object ---
echo "Building $assembly..."
clang $cFilenames $compilerFlags -o $OUTPUT_DLL $defines $includeFlags $linkerFlags
# Invoke clang with the collected source files and compiler/linker flags
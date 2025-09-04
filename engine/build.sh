#!/bin/bash
# Build script for engine

# --- Check required environment variables ---
for var in WORKSPACE BIN_DIR VULKAN_SDK; do
    if [ -z "${!var}" ]; then
        echo "[ERROR]: $var not defined. Please set $var environment variable before building!"
        exit 1
    fi
done

# --- Create BIN_DIR if it doesn't exist ---
if [ ! -d "$BIN_DIR" ]; then
    mkdir -p "$BIN_DIR"
    echo "Folder '$BIN_DIR' created."
fi

# --- Get a list of all the .c files ---
cFilenames=$(find . -type f -name "*.c")

# echo "Files:$cFilenames"

assembly="engine"
compilerFlags="-g -shared -fdeclspec -fPIC"
includeFlags="-Isrc -I$VULKAN_SDK/include"
linkerFlags="-L$VULKAN_SDK/lib -L/usr/lib/x86_64-linux-gnu -lvulkan -lxcb -lX11 -lX11-xcb -lxkbcommon"
defines="-D_DEBUG -DKEXPORT"

# --- Output file ---
OUTPUT_DLL="$BIN_DIR/lib$assembly.so"

echo "Building $assembly..."
clang $cFilenames $compilerFlags -o $OUTPUT_DLL $defines $includeFlags $linkerFlags

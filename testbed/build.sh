#!/bin/bash
# Build script for testbed

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

assembly="testbed"
compilerFlags="-g -fdeclspec -fPIC"
includeFlags="-Isrc -I$WORKSPACE/engine/src"
linkerFlags="-L$BIN_DIR -lengine -Wl,-rpath,."
defines="-D_DEBUG -DKIXPORT"

# --- Output file ---
OUTPUT_EX="$BIN_DIR/$assembly" 

echo "Building $assembly..."
clang $cFilenames $compilerFlags -o $OUTPUT_EX $defines $includeFlags $linkerFlags

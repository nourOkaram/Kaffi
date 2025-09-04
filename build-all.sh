#!/bin/bash
# Build everything

echo "Building everything..."

assemblies=("engine" "testbed")

for assembly in "${assemblies[@]}"; do
    pushd $assembly > /dev/null
    source build.sh
    ERRORLEVEL=$?
    popd > /dev/null
    
    if [ $ERRORLEVEL -ne 0 ]; then
        echo "[ERROR]: $ERRORLEVEL"
        exit $ERRORLEVEL
    fi
done

echo "ALL assemblies built successfully"

#!/bin/bash
# Master build script for the entire project on Linux.
# This script orchestrates the build process by calling the individual
# build scripts for each component in the correct order.

echo "Building everything..."

# Define an array of components (assemblies) to build.
# The order is critical: 'engine' must be built before 'testbed' because
# testbed depends on the shared object (libengine.so) created by the engine build.
assemblies=("engine" "testbed")

# Loop through each assembly defined in the 'assemblies' array.
for assembly in "${assemblies[@]}"; do

    # Temporarily change the current directory to the assembly's folder (e.g., 'engine').
    # Redirect stdout to /dev/null to keep the console output clean.
    pushd $assembly > /dev/null

    # Execute the build script located in that directory.
    # Using 'source' runs the script in the current shell context.
    source build.sh

    # Capture the exit code of the last command (build.sh).
    ERRORLEVEL=$?

    # Return to the previous directory.
    popd > /dev/null
    
    # Check the exit code of the build script.
    # A non-zero code indicates that the build failed.
    if [ $ERRORLEVEL -ne 0 ]; then
        echo "[ERROR]: Build failed in '${assembly}' with error code: ${ERRORLEVEL}"
        echo "Halting the build process."

        # Exit the entire script, passing the error code up.
        exit $ERRORLEVEL
    fi
done

# If the loop completes without any errors, all components were built successfully.
echo "ALL assemblies built successfully"

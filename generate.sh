#!/bin/bash

# Get the version argument
VERSION=$1

if [ -z "$VERSION" ]; then
    echo "Error: No version provided. Usage: ./generate.sh <version>"
    exit 1
fi

# Set the build directory
BUILD_DIR="build_linux"

# Function to build the project
build_project() {
    local build_system=$1
    local build_mode=$2

    # Ensure the build directory exists or create it
    if [ -d "$BUILD_DIR" ]; then
        echo "Build directory exists. Emptying it..."
        rm -rf "$BUILD_DIR"
    fi
    mkdir -p "$BUILD_DIR"

    cd "$BUILD_DIR" || exit

    echo "Running CMake with Makefiles..."
    cmake .. -G "Unix Makefiles"

    echo "Building the project in $build_mode mode..."
    cmake --build . --config "$build_mode"

    # Construct the installation path
    INSTALL_DIR="../output/artifact/v$VERSION/Linux64/$build_system/$build_mode"
    echo "Installing the project to $INSTALL_DIR..."
    cmake --install . --prefix "$INSTALL_DIR" --config "$build_mode"

    echo "Build for $build_system in $build_mode mode completed."

    cd ..
}

# Build with GCC in Release mode
build_project "GCC" "Release"

# Build with GCC in Debug mode
build_project "GCC" "Debug"

echo "All operations completed."

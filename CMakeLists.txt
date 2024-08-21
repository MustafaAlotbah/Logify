cmake_minimum_required(VERSION 3.20)
project(lib_Logify VERSION 0.1.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Set compiler flags based on build type
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Configuring for a Debug build")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g3 -Og")
    add_definitions(-DDEBUG_MODE)
else()
    message(STATUS "Configuring for a Release build")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g0 -O3")
endif()


# Define the platform-specific paths and packaging configurations.
# This ensures that the correct dependencies are used depending on the platform.
if(WIN32)

    # Set the dependency folder for Windows x64.
    set(DEPENDENCY_FOLDER "${CMAKE_SOURCE_DIR}/dependencies/win_x86_64/")

    # Use ZIP as the packaging format for Windows.
    set(CPACK_GENERATOR "ZIP")


elseif (UNIX AND NOT APPLE)
    # Set the dependency folder for Linux x64.
    set(DEPENDENCY_FOLDER "${CMAKE_SOURCE_DIR}/dependencies/linux_x86_64/")

    # Use TGZ (tarball gzip) as the packaging format for Linux.
    set(CPACK_GENERATOR "TGZ")

else()

    # Fatal error if the platform is unsupported.
    message(FATAL_ERROR "Unsupported Platform: ${CMAKE_SYSTEM_NAME}")

endif()

# Set the CMake prefix path to include platform-specific dependency directories.
# This helps CMake locate the required libraries and packages.
set(CMAKE_PREFIX_PATH
        "${DEPENDENCY_FOLDER}/pkgconfig/"
        "${DEPENDENCY_FOLDER}/Catch2/"
)


# Find required packages
find_package(PkgConfig REQUIRED)


# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/lib/$<CONFIG>")


# Include subdirectories for building
add_subdirectory(Logify)
add_subdirectory(tests)
add_subdirectory(examples)
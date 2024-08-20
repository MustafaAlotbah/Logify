# Define the Logify library target
add_library(Logify SHARED)

# Add sources to the Logify library target
target_sources(Logify PRIVATE
        source/Logify.cpp
        source/Logify_internal.cpp
        # Add more files as needed
)

# Pass the version to the source code via a preprocessor definition
target_compile_definitions(Logify PRIVATE LOGIFY_VERSION="${PROJECT_VERSION}")
target_compile_definitions(Logify PRIVATE BUILDING_LOGIFY="1")

# Specify the include directories for the target
target_include_directories(Logify
    PUBLIC
        # Use absolute path for building
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>

        # Use relative path for installation
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

# Find and link the required libraries below
#find_library(SOMELIBRARY_LIB NAMES somelibrary libsomelibrary)
#target_link_libraries(Logify PRIVATE ${SOMELIBRARY_LIB})
#
## Optionally, you can check if the library was found and provide a useful error message
#if (NOT SOMELIBRARY_LIB)
#    message(FATAL_ERROR "some library static library not found.")
#endif()


# Hide Internal functions
target_compile_options(Logify PRIVATE -fvisibility=hidden)
set_target_properties(Logify PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(Logify PROPERTIES VISIBILITY_INLINES_HIDDEN YES)

# Installation Rules
# Define the installation directories
install(TARGETS Logify
        EXPORT LogifyTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)


# Install the public headers
install(DIRECTORY ../include/ DESTINATION include)

# Install the package configuration files
install(EXPORT LogifyTargets
        FILE LogifyTargets.cmake
        NAMESPACE Logify::
        DESTINATION lib/cmake/Logify
)

# Create and install the package configuration files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/LogifyConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
        "../cmake/LogifyConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/LogifyConfig.cmake"
        INSTALL_DESTINATION lib/cmake/Logify
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/LogifyConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/LogifyConfigVersion.cmake"
        DESTINATION lib/cmake/Logify
)


# Set the package name and version
set(CPACK_PACKAGE_NAME "Logify")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "Mustafa Alotbah <mustafa.alotbah@gmail.com>")

# CPACK_GENERATOR set by parent CMakeLists.txt
include(CPack)
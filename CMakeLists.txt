# Communication Library - Root CMake Configuration
cmake_minimum_required(VERSION 3.16)
project(CommunicationLibrary VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build options
option(BUILD_TESTS "Build test suite" ON)
option(BUILD_EXAMPLES "Build examples" OFF)

# Include directories
include_directories(include)

# Add subdirectories
if(BUILD_TESTS)
    add_subdirectory(test)
    # Enable testing at root level
    enable_testing()
endif()

# Library target (placeholder for now)
# add_library(CommunicationLibrary INTERFACE)
# target_include_directories(CommunicationLibrary INTERFACE include)

# Install rules
install(DIRECTORY include/ DESTINATION include)
install(FILES LICENSE README.md DESTINATION share/doc/CommunicationLibrary)

# Export package configuration (commented out for now)
# include(CMakePackageConfigHelpers)
# write_basic_package_version_file(
#     "${CMAKE_CURRENT_BINARY_DIR}/CommunicationLibraryConfigVersion.cmake"
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY AnyNewerVersion
# )

# Package configuration (commented out for now)
# configure_package_config_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommunicationLibraryConfig.cmake.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/CommunicationLibraryConfig.cmake"
#     INSTALL_DESTINATION lib/cmake/CommunicationLibrary
# )

# install(FILES
#     "${CMAKE_CURRENT_BINARY_DIR}/CommunicationLibraryConfig.cmake"
#     "${CMAKE_CURRENT_BINARY_DIR}/CommunicationLibraryConfigVersion.cmake"
#     DESTINATION lib/cmake/CommunicationLibrary
# )

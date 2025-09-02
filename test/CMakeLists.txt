# Test configuration for Communication Library
cmake_minimum_required(VERSION 3.16)
project(CommunicationLibraryTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Unity test framework
include(FetchContent)
FetchContent_Declare(
    Unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG master
)
FetchContent_MakeAvailable(Unity)

# Create test executable
add_executable(contract_tests
    main.cpp
    contracts/RestInterfaceContractTests.cpp
)

# Include directories
target_include_directories(contract_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Unity_SOURCE_DIR}
)

# Link with Unity
target_link_libraries(contract_tests PRIVATE unity)

# Enable testing
enable_testing()

# Add test
add_test(NAME ContractTests COMMAND contract_tests)

# Set test properties
set_tests_properties(ContractTests PROPERTIES
    TIMEOUT 30
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Custom target for running tests
add_custom_target(run_contract_tests
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/contract_tests
    DEPENDS contract_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running contract tests..."
)

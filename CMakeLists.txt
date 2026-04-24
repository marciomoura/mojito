cmake_minimum_required(VERSION 3.28)

project(
    mojito
    VERSION 0.1.0
    DESCRIPTION "A modular C++ library for units and math"
    HOMEPAGE_URL "https://github.com/marciomoura/mojito/"
    LANGUAGES CXX
)

# Output directory configuration
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies")

# Standard C++ Configuration
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build Options
option(MOJITO_BUILD_TESTING "Build the testing tree" ON)
option(MOJITO_BUILD_EXAMPLES "Build examples" ON)
option(MOJITO_ENABLE_CCACHE "Enable ccache" ON)
option(MOJITO_ENABLE_CPPCHECK "Enable static analysis with cppcheck" ON)

# Check for recommended generator
message(STATUS "Generator: ${CMAKE_GENERATOR}")
if(NOT CMAKE_GENERATOR MATCHES "Ninja" AND NOT MSVC)
    message(WARNING "Using ${CMAKE_GENERATOR} - Ninja is recommended for best performance.")
endif()

# Enable CCache if available
if(MOJITO_ENABLE_CCACHE)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
        message(STATUS "CCache enabled")
    else()
        message(WARNING "CCache not found, proceeding without it.")
    endif()
endif()

# Static analysis with cppcheck
if(MOJITO_ENABLE_CPPCHECK)
    include(cppcheck)
endif()

# Testing Setup
if(MOJITO_BUILD_TESTING)
    message(STATUS "Testing enabled")
    include(gtest)
    enable_testing()
endif()

# Core Library
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mojito/CMakeLists.txt")
    add_subdirectory(mojito)
endif()

# Examples
if(MOJITO_BUILD_EXAMPLES AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mojito/examples")
    message(STATUS "Examples enabled")
    add_subdirectory(mojito/examples)
endif()

# Tests
if(MOJITO_BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mojito/tests")
    message(STATUS "Tests enabled")
    add_subdirectory(mojito/tests)
endif()

message(STATUS "Mojito configuration complete")

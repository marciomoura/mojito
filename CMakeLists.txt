cmake_minimum_required(VERSION 3.28)

message(
    STATUS
        [[

         __    __  ______      __  __  ______  ______
        /\ "-./  \/\  __ \    /\ \/\ \/\__  _\/\  __ \
        \ \ \-./\ \ \ \/\ \  _\_\ \ \ \/_/\ \/\ \ \/\ \
         \ \_\ \ \_\ \_____\/\_____\ \_\ \ \_\ \ \_____\
          \/_/  \/_/\/_____/\/_____/\/_/  \/_/  \/_____/

          Power electronics utilities 🍹
]])

project(
    mojito
    VERSION 0.1.0
    DESCRIPTION "A modular C++ library for units and math"
    HOMEPAGE_URL "https://github.com/marciomoura/mojito/"
    LANGUAGES CXX)

# --- Output Configuration ---
# Keeping things organized so we don't have files everywhere fr
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies")

# --- Standard Configuration ---
# Modules require C++20 or C++23. We're going 20 because we stay modern 💅
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Clean code only
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Build Options ---
option(MOJITO_BUILD_TESTING "Build the testing tree" ON)
option(MOJITO_BUILD_EXAMPLES "Build examples" ON)
option(MOJITO_ENABLE_CCACHE "Enable ccache" ON)

# --- Module Scanning Check ---
# C++ Modules work best with Ninja. If they aren't using it, we let them know.
if(NOT
   CMAKE_GENERATOR
   STREQUAL
   "Ninja"
   AND NOT MSVC)
    message(
        WARNING
            "Using ${CMAKE_GENERATOR} - Modules work best with Ninja, bestie. Consider -G Ninja."
    )
endif()

# --- Dependencies & Helpers ---
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(MOJITO_ENABLE_CCACHE)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
        message(STATUS "CCache enabled ✅")
    else()
        message(WARNING "CCache not found, proceeding without it.")
    endif()
endif()

# --- Testing Setup ---
if(MOJITO_BUILD_TESTING)
    message(STATUS "Testing enabled ✅")
    # Assuming you have a gtest.cmake in your cmake folder or you're using
    include(gtest)
    enable_testing()
else()
    message(STATUS "Testing disabled ❌")
endif()

# --- Core Library ---
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mojito/CMakeLists.txt")
    add_subdirectory(mojito)
endif()

# --- Examples ---
if(MOJITO_BUILD_EXAMPLES)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mojito/examples")
        message(STATUS "Examples enabled ✅")
        # add_subdirectory(examples)
    else()
        message(STATUS "Examples disabled ❌")
    endif()
endif()

# --- Tests ---
if(MOJITO_BUILD_TESTING)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mojito/tests")
        message(STATUS "Tests enabled ✅")
        add_subdirectory(mojito/tests)
    else()
        message(STATUS "Tests disabled ❌")
    endif()
endif()

message(STATUS "Mojito config complete 🍹")

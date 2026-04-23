# Description: Fetch the google-test framework (https://github.com/google/googletest)

include(FetchContent)

set(FETCHCONTENT_QUIET ON)

fetchcontent_declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        EXCLUDE_FROM_ALL)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)

# Suppress warnings from GoogleTest
set(OLD_WARN_DEPRECATED ${CMAKE_WARN_DEPRECATED})
set(CMAKE_WARN_DEPRECATED
    OFF
    CACHE BOOL "" FORCE)

fetchcontent_makeavailable(googletest)

set(CMAKE_WARN_DEPRECATED
    ${OLD_WARN_DEPRECATED}
    CACHE BOOL "" FORCE)

if(NOT TARGET gtest::gtest)
    add_library(gtest::gtest ALIAS gtest)
    add_library(gtest::gmock ALIAS gmock)
    add_library(gtest::gtest_main ALIAS gtest_main)
    add_library(gtest::gmock_main ALIAS gmock_main)
endif()

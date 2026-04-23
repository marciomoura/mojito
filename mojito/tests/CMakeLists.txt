project(mojito_tests CXX)

# Create the test executable
add_executable(${PROJECT_NAME})
add_executable(mojito::mojito_tests ALIAS ${PROJECT_NAME})

enable_testing()

# Add test sources
set(TEST_SOURCES)
foreach(test_file angle_test.cpp per_unit_test.cpp divisor_test.cpp reference_frame_test.cpp units_test.cpp)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${test_file}")
        list(APPEND TEST_SOURCES ${test_file})
    endif()
endforeach()

if(TEST_SOURCES)
    target_sources(${PROJECT_NAME} PRIVATE ${TEST_SOURCES})
else()
    # Add a dummy source if no tests are present to keep CMake happy
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/dummy_test.cpp" "int main() { return 0; }")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/dummy_test.cpp")
endif()

# Link dependencies
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE gtest::gtest_main
            gtest::gtest
            gtest::gmock
            mojito::mojito)
# $<$<NOT:$<PLATFORM_ID:Windows>>:pthread>)

# Discover tests
include(GoogleTest)
gtest_discover_tests(
    ${PROJECT_NAME}
    PROPERTIES LABELS "unit" # Add labels for better organization
    TIMEOUT 30 # Add timeout for each test
)

# if(mojito_CODE_COVERAGE) message(STATUS "Enabling coverage reporting with
# gcovr") # These flags tell the compiler to add instrumentation for coverage
# tracking target_compile_options(${PROJECT_NAME} PRIVATE -fprofile-arcs
# -ftest-coverage) target_link_options(${PROJECT_NAME} PRIVATE --coverage)
# endif()

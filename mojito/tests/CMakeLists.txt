project(mojito_tests CXX)

# Create the test executable
add_executable(${PROJECT_NAME})
add_executable(mojito::mojito_tests ALIAS ${PROJECT_NAME})

enable_testing()

# Add test sources
target_sources(${PROJECT_NAME} PRIVATE 
    abc_test.cpp 
    alphabeta_test.cpp 
    dq_test.cpp 
    ab_bc_ca_test.cpp 
    transform_test.cpp
    angle_test.cpp 
    divisor_test.cpp 
    units_test.cpp)

# Enable cppcheck if available
if(CMAKE_CXX_CPPCHECK)
    set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CPPCHECK "${CMAKE_CXX_CPPCHECK}")
endif()

# Link dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE gtest::gtest_main gtest::gtest gtest::gmock mojito::mojito)

# Discover tests
include(GoogleTest)
gtest_discover_tests(
    ${PROJECT_NAME}
    PROPERTIES LABELS "unit"
    TIMEOUT 30
)

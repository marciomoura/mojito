project(mojito_examples CXX)

if(TARGET mojito_module)
    add_executable(example_module_usage module_usage.cpp)
    set_target_properties(example_module_usage PROPERTIES CXX_SCAN_FOR_MODULES ON)
    target_link_libraries(example_module_usage PRIVATE mojito::module)
endif()

add_executable(abc_to_alpha_beta_to_dq abc_to_alpha_beta_to_dq.cpp)
target_link_libraries(abc_to_alpha_beta_to_dq PRIVATE mojito::mojito)

add_executable(example_operators operators.cpp)
target_link_libraries(example_operators PRIVATE mojito::mojito)

add_executable(example_rotation rotation.cpp)
target_link_libraries(example_rotation PRIVATE mojito::mojito)

add_executable(example_unit_conversion unit_conversion.cpp)
target_link_libraries(example_unit_conversion PRIVATE mojito::mojito)

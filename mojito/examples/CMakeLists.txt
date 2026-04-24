project(mojito_examples CXX)

if(TARGET mojito_module)
    add_executable(example_module_usage module_usage.cpp)
    set_target_properties(example_module_usage PROPERTIES CXX_SCAN_FOR_MODULES ON)
    target_link_libraries(example_module_usage PRIVATE mojito::module)
endif()

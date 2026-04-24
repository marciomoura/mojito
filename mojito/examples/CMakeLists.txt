project(mojito_examples CXX)

if(TARGET mojito_module)
    add_executable(example_module_usage module_usage.cpp)
    target_link_libraries(example_module_usage PRIVATE mojito::module)
endif()

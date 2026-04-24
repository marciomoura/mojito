project(mojito CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(mojito::mojito ALIAS ${PROJECT_NAME})

target_sources(
    ${PROJECT_NAME}
    INTERFACE 
    FILE_SET HEADERS
    BASE_DIRS include
    FILES 
        include/mojito/ab_bc_ca.hpp
        include/mojito/abc.hpp
        include/mojito/alphabeta.hpp
        include/mojito/angle_wrapped.hpp
        include/mojito/constants.hpp
        include/mojito/divisor.hpp
        include/mojito/dq.hpp
        include/mojito/math.hpp
        include/mojito/mojito.hpp
        include/mojito/pu_conversion.hpp
        include/mojito/quantity_cast.hpp
        include/mojito/transform.hpp
        include/mojito/type_traits.hpp
        include/mojito/types.hpp
        include/mojito/units.hpp)

target_link_libraries(${PROJECT_NAME} INTERFACE)

# C++20 Module Companion Target
set(MOJITO_HAS_MODULE_SUPPORT OFF)
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.28")
    if((CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "16.0") OR
       (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "14.0") OR
       (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "19.34"))
        set(MOJITO_HAS_MODULE_SUPPORT ON)
    endif()
endif()

if(MOJITO_HAS_MODULE_SUPPORT)
    add_library(mojito_module STATIC)
    add_library(mojito::module ALIAS mojito_module)

    set_target_properties(mojito_module PROPERTIES CXX_SCAN_FOR_MODULES ON)

    target_link_libraries(mojito_module PRIVATE ${PROJECT_NAME})
    target_compile_features(mojito_module PUBLIC cxx_std_20)

    target_sources(
        mojito_module
        PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS modules
        FILES modules/mojito.cppm)
endif()

# if(mojito_ENABLE_CODE_ANALYSIS)
# get_target_property(TARGET_SOURCES
# ${PROJECT_NAME} SOURCES) add_custom_target( ${PROJECT_NAME}_cppcheck DEPENDS
# ${PROJECT_NAME} COMMAND ${CMAKE_CXX_CPPCHECK}
# ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_SOURCES} WORKING_DIRECTORY
# ${CMAKE_CURRENT_SOURCE_DIR} COMMENT "Static code analysis using cppcheck")
# endif()

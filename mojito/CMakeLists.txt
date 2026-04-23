project(mojito CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(mojito::mojito ALIAS ${PROJECT_NAME})

target_sources(
    ${PROJECT_NAME}
    INTERFACE 
    FILE_SET HEADERS
    BASE_DIRS include
    FILES 
        include/mojito/angle_wrapped.hpp
        include/mojito/constants.hpp
        include/mojito/divisor.hpp
        include/mojito/reference_frame.hpp
        include/mojito/type_traits.hpp
        include/mojito/types.hpp
        include/mojito/units.hpp)

target_link_libraries(${PROJECT_NAME} INTERFACE)

# if(mojito_ENABLE_CODE_ANALYSIS)
# get_target_property(TARGET_SOURCES
# ${PROJECT_NAME} SOURCES) add_custom_target( ${PROJECT_NAME}_cppcheck DEPENDS
# ${PROJECT_NAME} COMMAND ${CMAKE_CXX_CPPCHECK}
# ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_SOURCES} WORKING_DIRECTORY
# ${CMAKE_CURRENT_SOURCE_DIR} COMMENT "Static code analysis using cppcheck")
# endif()

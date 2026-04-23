project(mojito CXX)

add_library(${PROJECT_NAME} STATIC)
add_library(mojito::mojito ALIAS ${PROJECT_NAME})

set(MOJITO_HEADERS)
foreach(header angle_wrapped.hpp per_unit.hpp divisor.hpp reference_frame.hpp type_traits.hpp types.hpp units.hpp)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/mojito/${header}")
        list(APPEND MOJITO_HEADERS "include/mojito/${header}")
    endif()
endforeach()

set(MOJITO_SOURCES)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/types.cpp")
    list(APPEND MOJITO_SOURCES "src/types.cpp")
endif()

target_sources(
    ${PROJECT_NAME}
    PUBLIC ${MOJITO_HEADERS}
    PRIVATE ${MOJITO_SOURCES})

target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC)

# if(mojito_ENABLE_CODE_ANALYSIS) get_target_property(TARGET_SOURCES
# ${PROJECT_NAME} SOURCES) add_custom_target( ${PROJECT_NAME}_cppcheck DEPENDS
# ${PROJECT_NAME} COMMAND ${CMAKE_CXX_CPPCHECK}
# ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_SOURCES} WORKING_DIRECTORY
# ${CMAKE_CURRENT_SOURCE_DIR} COMMENT "Static code analysis using cppcheck")
# endif()

# Description: Enables the use of cppcheck for static code analysis.

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)

if(CMAKE_CXX_CPPCHECK)
    message(
        STATUS
            "-- yeet build-config: 🟢 using cppcheck package for ${CMAKE_SYSTEM_NAME}"
    )
else()
    message(
        WARNING
            "-- yeet build-config: 🟡 cppcheck package for ${CMAKE_SYSTEM_NAME} not found"
    )
endif()

if(CMAKE_CXX_CPPCHECK)
    list(
        APPEND
        CMAKE_CXX_CPPCHECK
        "--enable=all"
        "--inconclusive"
        "--inline-suppr"
        "--quiet"
        "--suppress=unmatchedSuppression"
        "--suppress=unusedFunction"
        "--template='{file}:{line}: warning: {id} ({severity}): {message}'")
endif()

# Description: Enables the use of cppcheck for static code analysis.

if(NOT MOJITO_ENABLE_CPPCHECK)
    return()
endif()

find_program(CPPCHECK_PROGRAM NAMES cppcheck)

if(CPPCHECK_PROGRAM)
    message(
        STATUS
            "-- yeet build-config: 🟢 using cppcheck package for ${CMAKE_SYSTEM_NAME}"
    )
    set(CMAKE_CXX_CPPCHECK "${CPPCHECK_PROGRAM}")
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
else()
    message(
        WARNING
            "-- yeet build-config: 🟡 cppcheck package for ${CMAKE_SYSTEM_NAME} not found"
    )
    unset(CMAKE_CXX_CPPCHECK CACHE)
endif()

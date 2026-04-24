# Description: fetch cpptrace library for stack trace support (host builds only)

message(
    STATUS
        "-- yeet build-config: 🟢 fetching cpptrace library for ${CMAKE_SYSTEM_NAME}"
)

include(FetchContent)

fetchcontent_declare(
    cpptrace
    GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
    GIT_TAG v0.7.3
    GIT_SHALLOW TRUE)

set(CPPTRACE_BUILD_TESTING
    OFF
    CACHE BOOL "" FORCE)
set(CPPTRACE_BUILD_EXAMPLES
    OFF
    CACHE BOOL "" FORCE)

fetchcontent_makeavailable(cpptrace)

# Description: Enables the use of CCACHE if it is installed on the system.
# If not found, check https://packages.msys2.org/package/mingw-w64-x86_64-ccache for Windows targets

find_program(CCACHE_FOUND ccache)

if(CCACHE_FOUND)
    message(
        STATUS
            "-- yeet build-config: 🟢 using ccache package for ${CMAKE_SYSTEM_NAME}"
    )
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
else()
    message(
        WARNING
            "-- yeet build-config: 🟡 ccache package for ${CMAKE_SYSTEM_NAME} not found"
    )
endif()

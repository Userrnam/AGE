find_package(OpenAL)
if (NOT ${OPENAL_FOUND})
    message("OpenAL not found")
    if (UNIX AND NOT APPLE)
        if ((${LINUX_DISTRO} STREQUAL "Ubuntu") OR ${LINUX_DISTRO} STREQUAL "Debian")
            message("Installing openal")
            message("sudo apt-get install libopenal-dev -y")
            execute_process(
                COMMAND bash -c "sudo apt-get install libopenal-dev -y"
                OUTPUT_VARIABLE OUT_VAR
            )
        else ()
            message("Unsupported distro " ${LINUX_DISTRO})
        endif ()
    endif ()
    find_package(OpenAL)
endif ()

find_package(OpenAL)
if (NOT ${OPENAL_FOUND})
    message("OpenAL not found")
    if (UNIX AND NOT APPLE)
        if ((${LINUX_DISTRO} STREQUAL "Ubuntu") OR ${LINUX_DISTRO} STREQUAL "Debian")
            message("Installing openal")
            message("sudo apt-get install libopenal-dev -y")
            execute_process(
                COMMAND bash -c "sudo apt-get install libopenal-dev -y"
                OUTPUT_VARIABLE OUT_VAR
            )
        else ()
            message("Unsupported distro " ${LINUX_DISTRO})
        endif ()
    endif ()
    find_package(OpenAL)
endif ()

file(GLOB sources
    "*.cpp"
)
add_library(audio STATIC ${sources})

if (APPLE)
target_link_libraries(audio "-framework OpenAL")
else (APPLE)
target_link_libraries(audio ${OPENAL_LIBRARY})
endif (APPLE)

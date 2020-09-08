# install glm
find_package(glm)
if (NOT ${GLM_FOUND})
    message("GLM not found")
    if (UNIX AND NOT APPLE)
        if ((${LINUX_DISTRO} STREQUAL "Ubuntu") OR ${LINUX_DISTRO} STREQUAL "Debian")
            message("Installing glm")
            message("sudo apt-get install libglm-dev -y")
            execute_process(
                COMMAND bash -c "sudo apt-get install libglm-dev -y"
                OUTPUT_VARIABLE OUT_VAR
            )
        else ()
            message("Unsupported distro " ${LINUX_DISTRO})
        endif ()
    endif ()
endif ()

include_directories("src" "include")

ADD_SUBDIRECTORY("src")
ADD_SUBDIRECTORY("test")

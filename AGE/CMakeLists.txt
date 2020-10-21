# install glm
find_package(glm)
if (NOT ${GLM_FOUND})
    message("glm not found")
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
            message(FATAL_ERROR "Manual library installation required")
        endif ()
    else ()
        message(FATAL_ERROR "Manual library installation required")
    endif ()
endif ()

#install glfw
find_package(glfw3)
if (NOT ${GLFW3_FOUND})
    message("glfw3 not found")
    if (UNIX AND NOT APPLE)
        if ((${LINUX_DISTRO} STREQUAL "Ubuntu") OR ${LINUX_DISTRO} STREQUAL "Debian")
            message("Installing glfw3")
            message("sudo apt-get install libglfw3-dev -y")
            execute_process(
                COMMAND bash -c "sudo apt-get install libglfw3-dev -y"
                OUTPUT_VARIABLE OUT_VAR
            )
        else ()
            message("Unsupported distro " ${LINUX_DISTRO})
            message(FATAL_ERROR "Manual library installation required")
        endif ()
    else ()
        message(FATAL_ERROR "Manual library installation required")
    endif()
endif()

#install freetype
find_package(Freetype)
if (NOT ${FREETYPE_FOUND})
    message("freetype not found")
    if (UNIX AND NOT APPLE)
        if ((${LINUX_DISTRO} STREQUAL "Ubuntu") OR ${LINUX_DISTRO} STREQUAL "Debian")
            message("Installing freetype")
            message("sudo apt-get install libfreetype6 libfreetype6-dev -y")
            execute_process(
                COMMAND bash -c "sudo apt-get install libfreetype6 libfreetype6-dev -y"
                OUTPUT_VARIABLE OUT_VAR
            )
        else ()
            message("Unsupported distro " ${LINUX_DISTRO})
            message(FATAL_ERROR "Manual library installation required")
        endif ()
    else ()
        message(FATAL_ERROR "Manual library installation required")
    endif()
endif()

ADD_SUBDIRECTORY("src")

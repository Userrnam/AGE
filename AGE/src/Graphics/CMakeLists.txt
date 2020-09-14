#install vulkan
find_package(Freetype)
if (NOT ${Vulkan_FOUND})
    message("freetype not found")
    if (UNIX AND NOT APPLE)
        if ((${LINUX_DISTRO} STREQUAL "Ubuntu") OR ${LINUX_DISTRO} STREQUAL "Debian")
            message("Installing vulkan")
            message("sudo apt-get install vulkan-tools libvulkan-dev vulkan-validationlayers-dev -y")
            execute_process(
                COMMAND bash -c "sudo apt-get install vulkan-tools libvulkan-dev vulkan-validationlayers-dev -y"
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

include_directories("${CMAKE_CURRENT_LIST_DIR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I ${CMAKE_CURRENT_LIST_DIR}")

file(GLOB sources
    "*.cpp"
    "Core/*.cpp"
    "Core/Utils/*.cpp"
    "Core/Window/*.cpp"
    "Memory/*.cpp"
    "MemoryHolders/*.cpp"
    "ObjectCreation/*.cpp"
    "ObjectCreation/Components/*.cpp"
    "Objects/*.cpp"
    "Rendering/*.cpp"
    "View/*.cpp"
)

find_package(Freetype)
find_package(glfw3)

add_library(graphics STATIC ${sources})
target_include_directories(graphics PRIVATE "$CMAKE_CURRENT_LIST_DIR")
target_compile_options(graphics PUBLIC "-I${CMAKE_CURRENT_LIST_DIR}")
if (APPLE)
target_link_libraries(graphics containers glfw vulkan utils freetype)
else ()
target_include_directories(graphics PUBLIC "${FREETYPE_INCLUDE_DIRS}" "${GLFW3_INCLUDE_DIR}")
target_link_libraries(graphics containers vulkan ${GLFW3_LIBRARY} rt m dl X11 pthread xcb Xau Xdmcp utils ${FREETYPE_LIBRARIES})
endif ()
target_include_directories(graphics PRIVATE "$CMAKE_CURRENT_LIST_DIR")

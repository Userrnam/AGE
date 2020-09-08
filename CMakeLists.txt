cmake_minimum_required(VERSION 3.10)

project("AGE")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# used to install library if not found
# Ubuntu, Debian
set(LINUX_DISTRO Ubuntu)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

include_directories("/usr/local/include" "/usr/include")
link_directories("/usr/local/lib")

# freetype path
if(APPLE)
    include_directories("/usr/local/Cellar/freetype/2.10.1/include/freetype2")
endif(APPLE)

add_subdirectory(AGE)

include_directories(
    "include"
    "Core/include"
    "External/include"
)

file (GLOB sources
    "src/*.cpp"
    "Core/src/*.cpp"
    "External/src/*.cpp"
)

add_library(core STATIC ${sources})
set_source_files_properties( "External/src/stb_impl.cpp" PROPERTIES COMPILE_FLAGS "-Ofast" )

if (APPLE)
target_link_libraries(core glfw vulkan)
elseif (LINUX)
target_link_libraries(core vulkan glfw3 rt m dl X11 pthread xcb Xau Xdmcp)
endif ()

add_subdirectory(Core)

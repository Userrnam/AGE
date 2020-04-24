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

add_library(graphics STATIC ${sources})
set_source_files_properties( "External/src/stb_impl.cpp" PROPERTIES COMPILE_FLAGS "-Ofast" )

if (APPLE)
target_link_libraries(graphics glfw vulkan)
else ()
target_link_libraries(graphics vulkan glfw3 rt m dl X11 pthread xcb Xau Xdmcp)
endif ()

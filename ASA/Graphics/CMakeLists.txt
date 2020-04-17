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
target_link_libraries(core vulkan.1 glfw vulkan.1 vulkan.1.2.131)

add_subdirectory(Core)

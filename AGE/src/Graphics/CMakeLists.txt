include_directories("${CMAKE_CURRENT_LIST_DIR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I ${CMAKE_CURRENT_LIST_DIR}")

# build shaders
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Shaders")

file(GLOB shader_sources
    "Shaders/*"
)

foreach(shader_source ${shader_sources})
    file(RELATIVE_PATH r_path "${CMAKE_CURRENT_LIST_DIR}" "${shader_source}")
    list(APPEND shader_outs "${CMAKE_CURRENT_BINARY_DIR}/${r_path}.spv")
    message("${CMAKE_CURRENT_BINARY_DIR}/${r_path}.spv")
endforeach(shader_source ${shader_sources})

foreach(shader_source ${shader_sources})
    file(RELATIVE_PATH r_path "${CMAKE_CURRENT_LIST_DIR}" "${shader_source}")
    file(RELATIVE_PATH r_base "${PROJECT_SOURCE_DIR}" "${shader_source}")
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${r_path}.spv"
        COMMAND glslc ${shader_source} -o "${CMAKE_CURRENT_BINARY_DIR}/${r_path}.spv"
        DEPENDS ${shader_source}
        COMMENT "Building shader ${r_base}.spv"
    )
endforeach(shader_source ${shader_sources})

add_custom_target(shaders DEPENDS ${shader_outs})
# end build shader

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
add_library(graphics STATIC ${sources})
target_include_directories(graphics PRIVATE "$CMAKE_CURRENT_LIST_DIR")
target_compile_options(graphics PUBLIC "-I${CMAKE_CURRENT_LIST_DIR}")
if (APPLE)
target_link_libraries(graphics containers glfw vulkan utils freetype)
else (APPLE)
target_link_libraries(graphics containers vulkan glfw3 rt m dl X11 pthread xcb Xau Xdmcp utils freetype)
endif (APPLE)
target_include_directories(graphics PRIVATE "$CMAKE_CURRENT_LIST_DIR")

add_dependencies(graphics shaders)

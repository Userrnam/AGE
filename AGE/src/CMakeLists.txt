include_directories("External")

# build shaders
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Shaders")

file(GLOB shader_sources
    "Graphics/Shaders/*"
)

foreach(shader_source ${shader_sources})
    file(RELATIVE_PATH r_path "${CMAKE_CURRENT_LIST_DIR}/Graphics" "${shader_source}")
    list(APPEND shader_outs "${CMAKE_CURRENT_BINARY_DIR}/${r_path}.spv")
    message("${CMAKE_CURRENT_BINARY_DIR}/${r_path}.spv")
endforeach(shader_source ${shader_sources})

foreach(shader_source ${shader_sources})
    file(RELATIVE_PATH r_path "${CMAKE_CURRENT_LIST_DIR}/Graphics" "${shader_source}")
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
    "Audio/*.cpp"
)
add_library(audio STATIC ${sources})
if (APPLE)
target_link_libraries(audio "-framework OpenAL")
else (APPLE)
target_link_libraries(audio openal)
endif (APPLE)

file(GLOB sources
    "Containers/*.cpp"
)
add_library(containers STATIC ${sources})

file(GLOB sources
    "Events/*.cpp"
)
add_library(events STATIC ${sources})

file(GLOB sources
    "External/*.cpp"
)
add_library(external STATIC ${sources})
set_source_files_properties( "External/stb_impl.cpp" PROPERTIES COMPILE_FLAGS "-Ofast" )

file(GLOB sources
    "Scene/*.cpp"
)
add_library(scene STATIC ${sources})
set_target_properties(scene PROPERTIES COMPILE_FLAGS "-I External")

file(GLOB sources
    "Utils/*.cpp"
)
add_library(utils STATIC ${sources})

include_directories("Graphics")

file(GLOB sources
    "Graphics/*.cpp"
    "Graphics/Core/*.cpp"
    "Graphics/Core/Utils/*.cpp"
    "Graphics/Core/Window/*.cpp"
    "Graphics/Memory/*.cpp"
    "Graphics/MemoryHolders/*.cpp"
    "Graphics/ObjectCreation/*.cpp"
    "Graphics/Objects/*.cpp"
    "Graphics/View/*.cpp"
)
add_library(graphics STATIC ${sources})
target_compile_definitions(graphics PRIVATE SHADER_PATH="${CMAKE_CURRENT_BINARY_DIR}/Shaders/" CMAKE_DEFINITION)
target_include_directories(graphics PRIVATE "Graphics/")
if (APPLE)
target_link_libraries(graphics containers glfw vulkan utils freetype)
else (APPLE)
target_link_libraries(graphics containers vulkan glfw3 rt m dl X11 pthread xcb Xau Xdmcp utils freetype)
endif (APPLE)

file (GLOB sources
    "*.cpp"
)
add_library(age STATIC ${sources})
target_link_libraries(age audio graphics utils scene external events)

add_dependencies(graphics shaders)
add_dependencies(age graphics)

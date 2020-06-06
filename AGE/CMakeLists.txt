# build utils
file(GLOB utils_sources
    "src/Utils/*.cpp"
)
add_library(utils STATIC ${utils_sources})
target_include_directories(utils PRIVATE
    "include/Utils"
)
# end build utils

# build graphics

# build shaders
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Shaders")

file(GLOB shader_sources
    "src/Graphics/Shaders/*"
)

foreach(shader_source ${shader_sources})
    file(RELATIVE_PATH r_path "${CMAKE_CURRENT_LIST_DIR}/src/Graphics" "${shader_source}")
    list(APPEND shader_outs "${CMAKE_CURRENT_BINARY_DIR}/${r_path}.spv")
    message("${CMAKE_CURRENT_BINARY_DIR}/${r_path}.spv")
endforeach(shader_source ${shader_sources})

foreach(shader_source ${shader_sources})
    file(RELATIVE_PATH r_path "${CMAKE_CURRENT_LIST_DIR}/src/Graphics" "${shader_source}")
    file(RELATIVE_PATH r_base "${PROJECT_SOURCE_DIR}" "${shader_source}")
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${r_path}.spv"
        COMMAND glslc ${shader_source} -o "${CMAKE_CURRENT_BINARY_DIR}/${r_path}.spv"
        DEPENDS ${shader_source}
        COMMENT "Building shader ${r_base}.spv"
    )
endforeach(shader_source ${shader_sources})

add_custom_target(shaders DEPENDS ${shader_outs})
# end build shaders

file (GLOB sources
    "src/Graphics/*.cpp"
    "src/Graphics/Core/*.cpp"
    "src/Graphics/External/*.cpp"
)

add_library(graphics STATIC ${sources})
set_source_files_properties( "src/Graphics/External/stb_impl.cpp" PROPERTIES COMPILE_FLAGS "-Ofast" )
target_compile_definitions(graphics PRIVATE SHADER_PATH="${CMAKE_CURRENT_BINARY_DIR}/Shaders/" CMAKE_DEFINITION)
target_include_directories(graphics PRIVATE
    "include"
    "include/Graphics"
    "include/Graphics/Core"
    "include/Graphics/External"
)

if (APPLE)
target_link_libraries(graphics glfw vulkan utils freetype)
else (APPLE)
target_link_libraries(graphics vulkan glfw3 rt m dl X11 pthread xcb Xau Xdmcp utils freetype)
endif (APPLE)
# end build graphics

# build audio
file(GLOB sources
    "src/Audio/*.cpp"
)
add_library(audio STATIC ${sources})
if (APPLE)
target_link_libraries(audio "-framework OpenAL")
else (APPLE)
target_link_libraries(audio openal)
endif (APPLE)
target_include_directories(audio PRIVATE
    "include/Audio"
)
# end build audio

# build age
include_directories("include")
file (GLOB sources
    "src/*.cpp"
)
add_library(age STATIC ${sources})
target_include_directories(age PRIVATE
    "include/Graphics"
)
target_link_libraries(age graphics audio)
# end build age

# build test
add_executable(tst test/src/main.cpp)
target_include_directories(tst PRIVATE
    "test/include"
)
target_link_libraries(tst age)
target_compile_definitions(tst PRIVATE RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/test/Resources/" CMAKE_DEFINITION)
# end build test

# dependencies
add_dependencies(graphics shaders)
add_dependencies(age graphics)
add_dependencies(tst age)

# save exe path
file(RELATIVE_PATH exe_path ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
file(WRITE ${PROJECT_SOURCE_DIR}/.build/exe_path.txt ".build/${exe_path}/tst")

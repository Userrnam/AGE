# build graphics
file (GLOB sources
    "src/Graphics/*.cpp"
    "src/Graphics/Core/*.cpp"
    "src/Graphics/External/*.cpp"
)

add_library(graphics STATIC ${sources})
set_source_files_properties( "src/Graphics/External/stb_impl.cpp" PROPERTIES COMPILE_FLAGS "-Ofast" )

target_include_directories(graphics PRIVATE
    "include/Graphics"
    "include/Graphics/Core"
    "include/Graphics/External"
)

if (APPLE)
target_link_libraries(graphics glfw vulkan)
else ()
target_link_libraries(graphics vulkan glfw3 rt m dl X11 pthread xcb Xau Xdmcp)
endif ()
# end build graphics

# build event
file (GLOB event_sources
    "src/Events/*.cpp"
)
add_library(event STATIC ${event_sources})
target_include_directories(event PRIVATE
    "include/Events"
)
# end build event

# build age
include_directories(include)
file (GLOB sources
    "src/*.cpp"
)
add_library(age STATIC ${sources})
target_include_directories(age PRIVATE Graphics/Core/include)
target_link_libraries(age graphics event)
# end build age

# build test
add_executable(tst test/src/main.cpp)
target_include_directories(tst PRIVATE Graphics)
target_link_libraries(tst age)
# end build test

# dependencies
add_dependencies(age graphics)
add_dependencies(tst age)

# save exe path
file(RELATIVE_PATH exe_path ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
file(WRITE ${PROJECT_SOURCE_DIR}/.build/exe_path.txt ".build/${exe_path}/tst")

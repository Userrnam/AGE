file(GLOB sources
    "*.cpp"
)
add_library(scene STATIC ${sources})
target_link_libraries(scene graphics audio)
add_dependencies(scene graphics audio)

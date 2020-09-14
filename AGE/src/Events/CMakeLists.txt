file(GLOB sources
    "*.cpp"
)
add_library(events STATIC ${sources})
target_link_libraries(events containers)

add_dependencies(events containers)

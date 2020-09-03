file(GLOB sources
    "*.cpp"
)
add_library(events STATIC ${sources})

file(GLOB sources
    "*.cpp"
)
add_library(utils STATIC ${sources})

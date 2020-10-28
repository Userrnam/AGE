file(GLOB sources
    "*.cpp"
)
add_library(external STATIC ${sources})
set_source_files_properties("External/stb_impl.cpp" PROPERTIES COMPILE_FLAGS "-Ofast" )
